/*
 * Flow.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Flow.h"

struct flow {
    in_addr ip;
    uint64_t nonce;
};

/**
 * Constructor for the Flow class.
 * @param f a packet flow received from a packet
 * @param flowSize the size of the packet flow
 */
Flow::Flow(struct flow* f, int flowSize) {
	FlowEntry newFlowEntry;
	//unsigned char ipAddressBytes[4];

	for (int i = 0; i < flowSize; i++) {
		char ipaddrcstring[20];
		inet_ntop(AF_INET, &f[i].ip, ipaddrcstring, 20);
		char *tok = strtok(ipaddrcstring, ".");
		newFlowEntry.ipaddr[0] = atoi(tok);
		tok = strtok(NULL, ".");
		newFlowEntry.ipaddr[1] = atoi(tok);
		tok = strtok(NULL, ".");
		newFlowEntry.ipaddr[2] = atoi(tok);
		tok = strtok(NULL, ".");
		newFlowEntry.ipaddr[3] = atoi(tok);
		//ipAddressBytes[0] = (unsigned char*) ((&f[i].ip >> 24) & 0xFF);
		newFlowEntry.nonce = f[i].nonce;

		flowlist.push_back(newFlowEntry);
	}
}

Flow::~Flow() {
	
}

flow* Flow::getFlow() {
	struct flow* result;
	for (unsigned int i = 0; i < flowlist.size(); i++) {
		char ipaddrstring[20];
		sprintf(ipaddrstring, "%d.%d.%d.%d",flowlist[i].ipaddr[0],flowlist[i].ipaddr[1],flowlist[i].ipaddr[2],flowlist[i].ipaddr[3]);
		inet_pton(AF_INET, ipaddrstring, (void *) &result[i].ip);
		//result[i].ip = (in_addr) (flowlist[i].ipaddr[0] << 24) + (flowlist[i].ipaddr[1] << 16) + (flowlist[i].ipaddr[2] << 8) + flowlist[i].ipaddr[3];
		result[i].nonce = (uint64_t) flowlist[i].nonce;
	}
	return result;
}

bool Flow::operator==(const Flow& f) {
	bool isEqual = true;
	if(flowlist.size() != f.flowlist.size()){
			return false;
	}
	for (unsigned int i = 0; i < f.flowlist.size(); i++) {
		if (f.flowlist[i].ipaddr != flowlist[i].ipaddr) {
			isEqual = false;
		}
		if (f.flowlist[i].nonce != flowlist[i].nonce) {
			isEqual = false;
		}
	}
	return isEqual;
}

bool Flow::operator!=(const Flow& f) {
	bool isNotEqual = true;
	if(flowlist.size() != f.flowlist.size()){
		return true;
	}
	for (unsigned int i = 0; i < f.flowlist.size(); i++) {
		if (f.flowlist[i].ipaddr == flowlist[i].ipaddr) {
			isNotEqual = false;
		}
		if (f.flowlist[i].nonce == flowlist[i].nonce) {
			isNotEqual = false;
		}
	}
	return isNotEqual;
}

FlowEntry Flow::getAttackHost() {
	FlowEntry result;
	if (flowlist.size() >= 4) {
		result = flowlist[0];
	}
	return result;
}

FlowEntry Flow::getAttackGateway() {
	FlowEntry result;
	if (flowlist.size() >= 4) {
		result = flowlist[1];
	}
	return result;
}

FlowEntry Flow::getVictimGateway() {
	FlowEntry result;
	if (flowlist.size() >= 4) {
		result = flowlist[flowlist.size()-2];
	}
	return result;
}

FlowEntry Flow::getVictimHost() {
	FlowEntry result;
	if (flowlist.size() >= 4) {
		result = flowlist[flowlist.size()-1];
	}
	return result;
}

int Flow::size(){
	return flowlist.size();
}
