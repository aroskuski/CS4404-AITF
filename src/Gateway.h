/*
 * Gateway.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef GATEWAY_H_
#define GATEWAY_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include "Flow.h"
#include "Filter.h"

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
