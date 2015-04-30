//============================================================================
// Name        : CS4404-AITF.cpp
// Author      : 
// Version     :
// Copyright   : Your copyright notice
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <iostream>
#include "Gateway.h"
using namespace std;

int main() {
	Gateway gateway = Gateway();
	Filter filter = Filter(gateway.st);
	//filter.startFilterThread(false);

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
	std::string ipaddr = std::string(fe.ipaddr[0]) + "." + std::string(fe.ipaddr[1]) + "." + std::string(fe.ipaddr[2]) + "." + std::string(fe.ipaddr[3]) + ".";
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
	int sock = (long) arg;
	char buf[1500];
	int len = recv(sock, buf, 1500, 0);
	AITFHeader *h = (AITFHeader *)buf;



	return NULL;
}

void *gatewayTaskThread(void *arg){
	Gateway &g = *arg;
	sem_init(&Gateway::sem, 0 , 0);
	sem_init(&Gateway::qsem, 0 , 1);

	for(;;){
		sem_wait(&Gateway::sem);
		sem_wait(&Gateway::qsem);
		int id = Gateway::q.front();
		Gateway::q.pop();
		sem_post(&Gateway::qsem);
		g.escalate(id);
		sem_wait(&Gateway::qsem);
		Gateway::q.pop();
		sem_post(&Gateway::qsem);
	}

	return NULL;
}

void Gateway::tempBlock(){

}

void Gateway::remTempBlock(){

}

void Gateway::escalate(int id){

}

bool Gateway::checkBlacklist(){
	return false;
}

void Gateway::sendMessage(int id){
	sem_wait(&qsem);
	q.push(id);
	sem_post(&qsem);
	sem_post(&sem);
}
