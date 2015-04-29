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

	pthread_create(&taskThread, NULL, &hostTaskThread, NULL);

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


	return NULL;
}

Host::Host() {
	st = ShadowTable();

}

Host::~Host() {
	// TODO Auto-generated destructor stub
}

void Host::sendBlockReq(Flow f){
	char msg[1024];
	int msglen = 1023;

	addrinfo hints, *res;
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_DGRAM;
	getaddrinfo("0", "1025", &hints, &res);
	int len = sendto(Host::s, msg, msglen, 0, res->ai_addr,  res->ai_addrlen);

	freeaddrinfo(res);


}

void Host::honorBlockReq(std::string dest_addr){
	unsigned char h[4];
	this->st.addHost(h);
}
