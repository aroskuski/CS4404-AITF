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

int main(){
	Host host = Host();
	HostFilter filter = HostFilter();
	//filter.startFilterThread(true);
	pthread_t taskThread;
	pthread_t blockThread;

	//pthread_mutexattr_t mutexattr;
	//pthread_mutexattr_init(&mutexattr);
    //pthread_mutex_init(&Host::m, &mutexattr);

    sem_init(&Host::sem, 0 , 0);
    sem_init(&Host::qsem, 0 , 1);
    sem_init(&Host::lsem, 0, 1);

	pthread_create(&taskThread, NULL, &hostTaskThread, (void *) &host);
	pthread_create(&blockThread, NULL, &hostBlockCleanupThread, (void *) &host);

	Host::s = -1;
	while(Host::s < 0){
		Host::s = socket(AF_INET, SOCK_STREAM, 0);
	}

	addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "1025", &hints, &res);

    bind(Host::s, res->ai_addr, res->ai_addrlen);

    listen(Host::s,10);

    int len;
    char buf[1024];
    sockaddr_storage their_addr;
    for(;;){
    	unsigned int addr_size = sizeof(their_addr);
    	int sock = accept(Host::s, (sockaddr *) &their_addr, &addr_size);
    	pthread_t t;
    	pthread_create(&t, NULL, &hostRecvThread, (void *)sock);
    }

}

void *hostRecvThread(void *arg){
	int sock = (long) arg;
	char buf[1500];
	int len = recv(sock, buf, 1500, 0);
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
	char msg[1500];
	int msglen = sizeof(AITFHeader);

	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(ipaddr.c_str(), "1025", &hints, &res);
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
