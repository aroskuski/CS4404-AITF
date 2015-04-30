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
	unsigned char ipAddressBytes[4];
	for (int i = 0; i < flowSize; i++) {
		ipAddressBytes[0] = (unsigned char*) ((&f[i].ip >> 24) & 0xFF);
		newFlowEntry.nonce = (char*) f[i].nonce;
		flowlist.push_back(newFlowEntry);
	}
}

Flow::~Flow() {
	
}

flow* Flow::getFlow() {
	struct flow* result;
	for (int i = 0; i < flowlist.size(); i++) {
		result[i] = (uint32_t) flowlist[i].ipaddr;
		result[i] = (uint64_t) flowlist[i].nonce;
	}
	return result;
}

bool Flow::operator==(const Flow& f) {
	bool isEqual = true;
	for (int i = 0; i < f.flowlist.size(); i++) {
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
	for (int i = 0; i < f.flowlist.size(); i++) {
		if (f.flowlist[i].ipaddr == flowlist[i].ipaddr) {
			isNotEqual = false;
		}
		if (f.flowlist[i].nonce == flowlist[i].ipaddr) {
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
