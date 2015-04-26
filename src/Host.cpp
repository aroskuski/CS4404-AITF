/*
 * Host.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Host.h"

int main(){
	Filter filter = Filter();
	filter.startFilterThread(true);
	int s = -1;
	while(s < 0){
		s = socket(AF_INET, SOCK_DGRAM, 0);
	}

	addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_INET;
    hints.ai_socktype = SOCK_DGRAM;
    hints.ai_flags = AI_PASSIVE;

    getaddrinfo(NULL, "1025", &hints, &res);

	bind(s, res->ai_addr, res->ai_addrlen);

	//recv(s);

}

Host::Host() {
	// TODO Auto-generated constructor stub

}

Host::~Host() {
	// TODO Auto-generated destructor stub
}

void Host::sendBlockReq(Flow f){

	int s = -1;
	while(s < 0){
		s = socket(AF_INET, SOCK_DGRAM, 0);
	}



}
