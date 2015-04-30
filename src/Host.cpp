/*
 * Host.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Host.h"

int main(){
	Host host = Host();
	Filter filter = Filter(host.st);
	//filter.startFilterThread(true);
	pthread_t taskThread;

	pthread_mutexattr_t mutexattr;
	pthread_mutexattr_init(&mutexattr);
    pthread_mutex_init(&Host::m, &mutexattr);

	pthread_create(&taskThread, NULL, &hostTaskThread, (void *) &host);

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
    	accept(Host::s, (sockaddr *) &their_addr, &addr_size);
    }

}

void *hostTaskThread(void *arg){
	Host &h = *arg;
	sem_init(&Host::sem, 0 , 0);
	sem_init(&Host::qsem, 0 , 1);

	for(;;){
		sem_wait(&Host::sem);
		sem_wait(&Host::qsem);
		Flow f = Host::q.front();
		Host::q.pop();
		sem_post(&Host::qsem);
		h.sendBlockReq(f);
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
	FlowEntry fe = f.getVicimGateway();
	std::string ipaddr = std::string(fe.ipaddr[0]) + "." + std::string(fe.ipaddr[1]) + "." + std::string(fe.ipaddr[2]) + "." + std::string(fe.ipaddr[3]) + ".";
	char msg[1024];
	int msglen = 1023;

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

void Host::honorBlockReq(std::string dest_addr){
	unsigned char h[4];
	this->st.addHost(h);
}

void Host::sendMessage(Flow f){
	sem_wait(&qsem);
	q.push(f);
	sem_post(&qsem);
	sem_post(&sem);
}
