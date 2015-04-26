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
	Filter filter = Filter();
	filter.startFilterThread(false);

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
}

Gateway::Gateway() {
	// TODO Auto-generated constructor stub

}

Gateway::~Gateway() {
	// TODO Auto-generated destructor stub
}

void Gateway::sendBlockReq(Flow f){

}

void recvBlockReq(){

}

void Gateway::tempBlock(){

}

void Gateway::remTempBlock(){

}

void Gateway::escalate(){

}

bool Gateway::checkWhitelist(){
	return false;
}
