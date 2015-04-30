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
 * @param f a packet flow recieved from a packet
 * @param flowSize the size of the packet flow
 */
Flow::Flow(struct flow[6] f, int flowSize) {
	FlowEntry newFlowEntry;
	for (int i = 0; i < flowSize; i++) {
		newFlowEntry.ipaddr = (unsigned char[4]) f[i].ip; 
		newFlowEntry.nonce = (unsigned char[64]) f[i].nonce;
		flow.push_back(newFlowEntry);
	}
}

Flow::~Flow() {
	
}

bool Flow::operator==(const Flow& f) {
	bool isEqual = true;
	for (int i = 0; i < f.getFlow().size(); i++) {
		if (f[i].ipaddr != this.getFlow()[i].ipaddr) {
			isEqual = false;
		}
		if (f[i].nonce != this.getFlow()[i].nonce) {
			isEqual = false;
		}
	}
	return isEqual;
}

bool Flow::operator!=(const Flow& f) {
	bool isNotEqual = true;
	for (int i = 0; i < f.getFlow().size(); i++) {
		if (f[i].ipaddr == this.getFlow()[i].ipaddr) {
			isNotEqual = false;
		}
		if (f[i].nonce == this.getFlow()[i].ipaddr) {
			isNotEqual = false;
		}
	}
	return isNotEqual;
}

std::vector<FlowEntry> Flow::getFlow() {
	return flow;
}

FlowEntry Flow::getAttackHost() {
	FlowEntry result;
	if (flow.size() >= 4) {
		result = flow[flow.size() - 1];
	}
	else {
		result = NULL;
	}
	return result;
}

FlowEntry Flow::getVictimGateway() {
	FlowEntry result;
	if (flow.size() >= 4) {
		result = flow[1];
	}
	else {
		result = NULL;
	}
	return result;
}

