/*
 * Host.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Host.h"

sem_t Host::sem;
sem_t Host::qsem;
std::queue<Flow> Host::q = std::queue<Flow>();
std::list<hostblock> Host::blocklist = std::list<hostblock>();
sem_t Host::lsem;
int Host::s;

void *hostRecvThread(void *arg){
	int sock = (long) arg;
	char buf[PACKET_PAYLOAD_SIZE];
	int len = recv(sock, buf, PACKET_PAYLOAD_SIZE, 0);
	AITFHeader *h = (AITFHeader *)buf;
	Flow f = Flow(h->pktFlow, h->flowSize);
	FlowEntry fe = f.getVictimHost();
	sem_wait(&Host::lsem);
	hostblock hb;
	char ipaddrstring[20];
	sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
	hb.ipaddr = std::string(ipaddrstring);
	hb.ttl = 30;
	std::string tempstr = std::string("iptables -A OUTPUT ") + hb.ipaddr + std::string(" -j DROP");
	system(tempstr.c_str());
	Host::blocklist.push_back(hb);
	sem_post(&Host::lsem);

	//add to st
	return NULL;
}

void *hostBlockCleanupThread(void *arg){
	for(;;){
		sleep(1000);
		sem_wait(&Host::lsem);
		for(std::list<hostblock>::iterator it  =  Host::blocklist.begin(); it != Host::blocklist.end(); it++){
			it->ttl--;
			if(it->ttl == 0){
				std::string tempstr = std::string("iptables -D OUTPUT ") + it->ipaddr + std::string(" -j DROP");
				system(tempstr.c_str());
			}
		}
		sem_post(&Host::lsem);
	}

	return NULL;
}

void *hostTaskThread(void *arg){
	Host *h =(Host *) arg;


	for(;;){
		sem_wait(&Host::sem);
		sem_wait(&Host::qsem);
		Flow f = Host::q.front();
		sem_post(&Host::qsem);
		h->sendBlockReq(f);
		sem_wait(&Host::qsem);
		Host::q.pop();
		sem_post(&Host::qsem);
	}

	return NULL;
}

Host::Host() {
	st = ShadowTable();
	q = std::queue<Flow>();
}

Host::~Host() {
	// TODO Auto-generated destructor stub
}

void Host::sendBlockReq(Flow f){
	FlowEntry fe = f.getVictimGateway();
	char ipaddrstring[20];
	sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
	std::string ipaddr = std::string(ipaddrstring);
	char msg[PACKET_PAYLOAD_SIZE];
	int msglen = sizeof(AITFHeader);

	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(ipaddr.c_str(), PORT, &hints, &res);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, res->ai_addr, res->ai_addrlen);

	AITFHeader *h = (AITFHeader *)msg;
	h->commandFlags = FILTERING_REQUEST;
	for(unsigned int i = 0; i < f.size(); i++){
		h->pktFlow[i] = f.getFlow()[i];
	}
	//h->pktFlow = f.getFlow();
	h->flowSize = f.size();
	h->payloadSize = 0;

	int len = send(sock, msg, msglen, 0);


	shutdown(sock, 2);
	freeaddrinfo(res);
}

void Host::sendMessage(Flow f){
	sem_wait(&qsem);
	q.push(f);
	sem_post(&qsem);
	sem_post(&sem);
}
