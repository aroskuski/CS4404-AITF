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
	for (int i = 0; i < flowSize; i++) {
		newFlowEntry.ipaddr = (unsigned char[4]) &f[i].ip;
		newFlowEntry.nonce = (unsigned char[64]) &f[i].nonce;
		flow.push_back(newFlowEntry);
	}
}

Flow::~Flow() {
	
}

flow* Flow::getFlow() {
	struct flow* result;
	for (int i = 0; i < flow.size(); i++) {
		result[i] = (in_addr) flow[i].ipaddr;
		result[i] = (uint64_t) flow[i].nonce;
	}
	return result;
}

bool Flow::operator==(const Flow& f) {
	bool isEqual = true;
	for (int i = 0; i < f.flow.size(); i++) {
		if (f.flow[i].ipaddr != flow[i].ipaddr) {
			isEqual = false;
		}
		if (f.flow[i].nonce != flow[i].nonce) {
			isEqual = false;
		}
	}
	return isEqual;
}

bool Flow::operator!=(const Flow& f) {
	bool isNotEqual = true;
	for (int i = 0; i < f.flow.size(); i++) {
		if (f.flow[i].ipaddr == flow[i].ipaddr) {
			isNotEqual = false;
		}
		if (f.flow[i].nonce == flow[i].ipaddr) {
			isNotEqual = false;
		}
	}
	return isNotEqual;
}

FlowEntry Flow::getAttackHost() {
	FlowEntry result;
	if (flow.size() >= 4) {
		result = flow[flow.size() - 1];
	}
	else {
		result = nullptr;
	}
	return result;
}

FlowEntry Flow::getAttackGateway() {
	FlowEntry result;
	if (flow.size() >= 4) {
		result = flow[flow.size()-2];
	}
	else {
		result = nullptr;
	}
	return result;
}

FlowEntry Flow::getVictimGateway() {
	FlowEntry result;
	if (flow.size() >= 4) {
		result = flow[1];
	}
	else {
		result = nullptr;
	}
	return result;
}

