/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FLOW_H_
#define FLOW_H_

#include <vector>

/**
 * C structure for a packet flow
 * - Contains the ip address of a visited machine
 * - Contains the nonce for that visited machine
 */
struct flow {
    in_addr ip;
    uint64_t nonce;
};

struct FlowEntry {
	unsigned char ipaddr[4];
	unsigned char nonce[64];
};

class Flow {
public:
	Flow(struct flow* f, int flowSize);
	bool operator==(const Flow& f);
	bool operator!=(const Flow& f);
	FlowEntry getAttackHost();
	FlowEntry getAttackGateway();
	FlowEntry getVictimGateway();
	int size();
	virtual ~Flow();
	flow* getFlow();
	std::vector<FlowEntry> flow;
};

#endif /* FLOW_H_ */
