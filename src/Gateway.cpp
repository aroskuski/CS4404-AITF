//============================================================================
// Name        : CS4404-AITF.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

//#include <iostream>
#include "Gateway.h"
//using namespace std;

int Gateway::s;
sem_t Gateway::sem;
sem_t Gateway::qsem;
std::queue<Flow> Gateway::q = std::queue<Flow>();
std::list<gatewayblock> Gateway::blocklist = std::list<gatewayblock>();
sem_t Gateway::lsem;

int main() {
	Gateway gateway = Gateway();
	//GatewayFilter filter = GatewayFilter();
	//filter.startFilterThread(false);

	pthread_t taskThread;
	pthread_t blockThread;

	sem_init(&Gateway::sem, 0 , 0);
	sem_init(&Gateway::qsem, 0 , 1);
	sem_init(&Gateway::lsem, 0, 1);

	pthread_create(&taskThread, NULL, &gatewayTaskThread, (void *) &gateway);
	pthread_create(&blockThread, NULL, &gatewayBlockCleanupThread, (void *) &gateway);

	int s = -1;
	while(s < 0){
		s = socket(AF_INET, SOCK_STREAM, 0);
	}

	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;

	getaddrinfo(NULL, "1025", &hints, &res);

	bind(Gateway::s, res->ai_addr, res->ai_addrlen);

	listen(Gateway::s,10);

	int len;
	char buf[1024];
	sockaddr_storage their_addr;
	for(;;){
		unsigned int addr_size = sizeof(their_addr);
		int sock = accept(Gateway::s, (sockaddr *) &their_addr, &addr_size);
		pthread_t t;
		pthread_create(&t, NULL, &recvBlockReq, (void *)sock);
	}
}

Gateway::Gateway() {
	// TODO Auto-generated constructor stub
	st = ShadowTable();
}

Gateway::~Gateway() {
	// TODO Auto-generated destructor stub
}

void Gateway::sendBlockReq(Flow f){
	FlowEntry fe = f.getAttackHost();
	char ipaddrstring[20];
	sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
	std::string ipaddr = std::string(ipaddrstring);
	char msg[1500];
	int msglen = 1500;

	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	getaddrinfo(ipaddr.c_str(), "1025", &hints, &res);
	int sock = socket(AF_INET, SOCK_STREAM, 0);
	connect(sock, res->ai_addr, res->ai_addrlen);

	int len = send(sock, msg, msglen, 0);


	shutdown(sock, 2);
	freeaddrinfo(res);
}

void *recvBlockReq(void *arg){
	Gateway *g = (Gateway *) arg;
	int sock = (long) arg;
	char buf[1500];
	int len = recv(sock, buf, 1500, 0);
	shutdown(sock, 2);
	AITFHeader *h = (AITFHeader *)buf;
	Flow f = Flow(h->pktFlow, h->flowSize);
	//FlowEntry fe = f.getAttackGateway();
	//std::string ipaddr = std::string(fe.ipaddr[0]) + "." + std::string(fe.ipaddr[1]) + "." + std::string(fe.ipaddr[2]) + "." + std::string(fe.ipaddr[3]);
	if(h->commandFlags == 1){
		sock = socket(AF_INET, SOCK_STREAM, 0);
		FlowEntry fe = f.getAttackGateway();
		char ipaddrstring[20];
		sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
		std::string ipaddr = std::string(ipaddrstring);
		g->tempBlock(ipaddr);
		//char msg[1500];
		//int msglen = sizeof(AITFHeader);
		addrinfo hints, *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo(ipaddr.c_str(), "1025", &hints, &res);
		connect(sock, res->ai_addr, res->ai_addrlen);

		h->commandFlags = 2;

		send(sock, buf, sizeof(AITFHeader), 0);
		char buf2[1500];
		recv(sock, buf2, sizeof(AITFHeader), 0);
		g->remTempBlock(ipaddr);
		send(sock, buf2, sizeof(AITFHeader), 0);

		shutdown(sock, 2);
	} else if ( h->commandFlags == 2){
		FlowEntry fe = f.getAttackHost();
		char ipaddrstring[20];
		sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
		std::string ipaddr = std::string(ipaddrstring);
		unsigned long long nonce = 0;
		h->nonce = 0;
		send(sock, buf, sizeof(AITFHeader), 0);
		g->tempBlock(ipaddr);
		char buf2[1500];
		recv(sock, buf2, sizeof(AITFHeader), 0);
		AITFHeader *h2 = (AITFHeader *) buf2;
		if(h2->nonce == nonce){
			shutdown(sock, 2);
			sock = socket(AF_INET, SOCK_STREAM, 0);
			addrinfo hints, *res;
			memset(&hints, 0, sizeof(hints));
			hints.ai_family = AF_INET;
			hints.ai_socktype = SOCK_STREAM;
			getaddrinfo(ipaddr.c_str(), "1025", &hints, &res);
			connect(sock, res->ai_addr, res->ai_addrlen);
			h->nonce = 0;
			send(sock, buf, sizeof(AITFHeader), 0);
		}
		g->remTempBlock(ipaddr);
		shutdown(sock, 2);
	} else {
		char ipaddrcstring[20];
		inet_ntop(AF_INET, &h->pktFlow[h->payloadSize], ipaddrcstring, 20);
		std::string ipaddr = std::string(ipaddrcstring);
		g->tempBlock(ipaddr);
		unsigned long long nonce = 0;
		h->nonce = nonce;
		send(sock, buf, sizeof(AITFHeader), 0);
		char buf2[1500];
		recv(sock, buf2, sizeof(AITFHeader), 0);
		AITFHeader *h2 = (AITFHeader *) buf2;
		if(h2->nonce == nonce){
			sem_wait(&Gateway::lsem);
			gatewayblock gb;
			gb.ipaddr = ipaddr;
			gb.ttl = 30;
			std::string tempstr = std::string("iptables -A OUTPUT ") + gb.ipaddr + std::string(" -j DROP");
			system(tempstr.c_str());
			Gateway::blocklist.push_back(gb);
			sem_post(&Gateway::lsem);
		}
		g->remTempBlock(ipaddr);
		shutdown(sock, 2);
	}



	return NULL;
}

void *gatewayBlockCleanupThread(void *arg){
	for(;;){
		sleep(1000);
		sem_wait(&Gateway::lsem);
		for(std::list<gatewayblock>::iterator it  =  Gateway::blocklist.begin(); it != Gateway::blocklist.end(); it++){
			it->ttl--;
			if(it->ttl == 0){
				std::string tempstr = std::string("iptables -D INPUT ") + it->ipaddr + std::string(" -j DROP");
				system(tempstr.c_str());
			}
		}
		sem_post(&Gateway::lsem);
	}

	return NULL;
}

void *gatewayTaskThread(void *arg){
	Gateway *g = (Gateway *)arg;
	sem_init(&Gateway::sem, 0 , 0);
	sem_init(&Gateway::qsem, 0 , 1);

	for(;;){
		sem_wait(&Gateway::sem);
		sem_wait(&Gateway::qsem);
		Flow f = Gateway::q.front();
		Gateway::q.pop();
		sem_post(&Gateway::qsem);
		g->escalate(f);
		sem_wait(&Gateway::qsem);
		Gateway::q.pop();
		sem_post(&Gateway::qsem);
	}

	return NULL;
}

void Gateway::tempBlock(std::string ipaddr){
	sem_wait(&Gateway::lsem);
	gatewayblock gb;
	gb.ipaddr = ipaddr;
	gb.ttl = 999;
	std::string tempstr = std::string("iptables -A OUTPUT ") + gb.ipaddr + std::string(" -j DROP");
	system(tempstr.c_str());
	Gateway::blocklist.push_back(gb);
}

void Gateway::remTempBlock(std::string ipaddr){
	gatewayblock gb;
	gb.ipaddr = ipaddr;
	gb.ttl = 999;
	std::string tempstr = std::string("iptables -D OUTPUT ") + gb.ipaddr + std::string(" -j DROP");
	system(tempstr.c_str());
	Gateway::blocklist.remove(gb);
	sem_post(&Gateway::lsem);
}

void Gateway::escalate(Flow f){
	ShadowEntry *se = st.getshadowEntry(f);
	se->esccount++;
	if(f.getVictimGateway().ipaddr == f.getIndex(se->esccount + 1).ipaddr){//+1 to skip attack host
		FlowEntry fe = f.getAttackHost();
		sem_wait(&Gateway::lsem);
		gatewayblock gb;
		char ipaddrstring[20];
		sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
		std::string ipaddr = std::string(ipaddrstring);
		gb.ipaddr = ipaddr;
		gb.ttl = 30;
		std::string tempstr = std::string("iptables -A OUTPUT ") + gb.ipaddr + std::string(" -j DROP");
		system(tempstr.c_str());
		Gateway::blocklist.push_back(gb);
		sem_post(&Gateway::lsem);
	} else {
		FlowEntry fe = f.getIndex(se->esccount + 1);
		char ipaddrstring[20];
		sprintf(ipaddrstring, "%d.%d.%d.%d",fe.ipaddr[0],fe.ipaddr[1],fe.ipaddr[2],fe.ipaddr[3]);
		std::string ipaddr = std::string(ipaddrstring);
		int sock = socket(AF_INET, SOCK_STREAM, 0);
		addrinfo hints, *res;
		memset(&hints, 0, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		getaddrinfo(ipaddr.c_str(), "1025", &hints, &res);
		connect(sock, res->ai_addr, res->ai_addrlen);

		char msg[1500];
		AITFHeader *h = (AITFHeader *) msg;
		h->commandFlags = 3;
		h->flowSize = f.size();
		for(unsigned int i = 0; i < f.size(); i++){
			h->pktFlow[i] = f.getFlow()[i];
		}
		//h->pktFlow = f.getFlow();
		h->payloadSize = se->esccount;

		send(sock, msg, sizeof(AITFHeader), 0);
		char buf2[1500];
		recv(sock, buf2, sizeof(AITFHeader), 0);
		send(sock, buf2, sizeof(AITFHeader), 0);

		shutdown(sock, 2);
	}
}

bool Gateway::checkBlacklist(){
	return false;
}

void Gateway::sendMessage(Flow f){
	sem_wait(&qsem);
	q.push(f);
	sem_post(&qsem);
	sem_post(&sem);
}
