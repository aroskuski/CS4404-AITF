/*
 * Gateway.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef GATEWAY_H_
#define GATEWAY_H_
#include "Flow.h"

class Gateway {
public:
	Gateway();

	virtual ~Gateway();
private:
	void sendBlockReq(Flow f);
	void recvBlockReq();
	void tempBlock();
	void remTempBlock();
	void escalate();
	bool checkWhitelist();
};

#endif /* GATEWAY_H_ */