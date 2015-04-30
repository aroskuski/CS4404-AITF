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
#include "ShadowTable.h"
#include "Packet.h"


class Gateway {
public:
	Gateway();
	ShadowTable st;
	void sendMessage(int id);
	virtual ~Gateway();
	static int s;
	static sem_t sem;
	static sem_t qsem;
	static std::queue<Flow> q;
private:
	void sendBlockReq(Flow f);
	void recvBlockReq();
	void tempBlock();
	void remTempBlock();
	void escalate();
	bool checkBlacklist();
};

void *recvBlockReq(void * arg);
void *gatewayTaskThread(void *arg);

#endif /* GATEWAY_H_ */
