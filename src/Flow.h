/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FLOW_H_
#define FLOW_H_
#include <stdlib.h>
#include <vector>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <cstdio>
#include <string>
#include <cstring>
/**
 * C structure for a packet flow
 * - Contains the ip address of a visited machine
 * - Contains the nonce for that visited machine
 */


struct FlowEntry {
	unsigned char ipaddr[4];
	uint64_t nonce;
};

class Flow {
public:
	Flow(struct flow* f, int flowSize);
	bool operator==(const Flow& f);
	bool operator!=(const Flow& f);
	FlowEntry getAttackHost();
	FlowEntry getAttackGateway();
	FlowEntry getVictimGateway();
	FlowEntry getVictimHost();
	int size();
	virtual ~Flow();
	flow* getFlow();
	std::vector<FlowEntry> flowlist;
};

#endif /* FLOW_H_ */
