/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef __FLOW_H__
#define __FLOW_H__

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
	Flow();
	Flow(struct flow* f, int flowSize);
	bool operator==(const Flow& f) const;
	bool operator!=(const Flow& f) const;
	FlowEntry getAttackHost();
	FlowEntry getAttackGateway();
	FlowEntry getVictimGateway();
	FlowEntry getVictimHost();
	FlowEntry getIndex(int i);
	int size();
	virtual ~Flow();
	flow* getFlow();
	std::vector<FlowEntry> flowlist;
};

#endif /* __FLOW_H__ */
