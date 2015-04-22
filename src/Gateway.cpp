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
