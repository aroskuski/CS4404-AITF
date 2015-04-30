/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FLOW_H_
#define FLOW_H_

#include <vector>
#include <netinet/in.h>
/**
 * C structure for a packet flow
 * - Contains the ip address of a visited machine
 * - Contains the nonce for that visited machine
 */


struct FlowEntry {
	unsigned char ipaddr[4];
	unsigned char nonce[8];
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
