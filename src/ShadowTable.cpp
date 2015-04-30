/*
 * ShadowTable.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "ShadowTable.h"

ShadowTable::ShadowTable() {
	// TODO Auto-generated constructor stub
	hostList = std::list<ShadowEntry>();

}

ShadowTable::~ShadowTable() {
	// TODO Auto-generated destructor stub
}

void ShadowTable::addFlow(Flow f){

}

void ShadowTable::remFlow(Flow f){

}

bool ShadowTable::containsHost(unsigned char *h){
	return true;
}

bool ShadowTable::containsFlow(Flow f){
	return true;
}

void ShadowTable::clean(){

}

ShadowEntry *ShadowTable::getshadowEntry(Flow f){
	return NULL;
}
