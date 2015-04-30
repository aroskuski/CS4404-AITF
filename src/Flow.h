/*
 * Flow.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FLOW_H_
#define FLOW_H_

struct FlowEntry {
	unsigned char ipaddr[4];
	unsigned char nonce[64];
};

class Flow {
public:
	Flow();
	bool operator==(const Flow& f);
	bool operator!=(const Flow& f);
	FlowEntry getAttackHost();
	FlowEntry getAttackGateway();
	virtual ~Flow();
};

#endif /* FLOW_H_ */
