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

	sockaddr_in sa;
	memset(&sa, 0, sizeof(sockaddr_in));
	sa.sin_family = AF_INET;
	//sa.sin_addr.s_addr = inet_addr(SENDER_IP_ADDR);
	sa.sin_port = htons(1025);

	bind(s, (sockaddr *)&sa, sizeof(sockaddr_in));

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
