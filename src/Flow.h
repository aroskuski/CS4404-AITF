/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FLOW_H_
#define FLOW_H_

struct FlowEntry {
	char ipaddr[4];
	char nonce[64];
};

class Flow {
public:
	Flow();
	bool operator==(const Flow& lhs, const Flow& rhs);
	bool operator!=(const Flow& lhs, const Flow& rhs);
	FlowEntry getAttackHost();
	FlowEntry getAttackGateway();
	virtual ~Flow();
};

#endif /* FLOW_H_ */
