/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FLOW_H_
#define FLOW_H_

#include <vector>

struct FlowEntry {
	unsigned char ipaddr[4];
	unsigned char nonce[64];
};

class Flow {
public:
	Flow(std::vector<FlowEntry> f);
	bool operator==(const Flow& f);
	bool operator!=(const Flow& f);
	FlowEntry getAttackHost();
	FlowEntry getAttackGateway();
	FlowEntry getVicimGateway();
	virtual ~Flow();
	std::vector<FlowEntry> getFlow();
private:
	std::vector<FlowEntry> flow;
};

#endif /* FLOW_H_ */
