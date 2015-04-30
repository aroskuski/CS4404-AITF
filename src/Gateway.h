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
#include <string>
#include <cstring>
#include <pthread.h>
#include <semaphore.h>
#include <list>
#include <queue>
#include <arpa/inet.h>
#include "Flow.h"
#include "GatewayFilter.h"
#include "ShadowTable.h"
#include "Packet.h"
#include "Hash.h"

struct gatewayblock{
	std::string ipaddr;
	int ttl;
};

class Gateway {
public:
	Gateway();
	ShadowTable st;
	void sendMessage(Flow f);
	virtual ~Gateway();
	static int s;
	static sem_t sem;
	static sem_t qsem;
	static std::queue<Flow> q;
	static std::list<gatewayblock> blocklist;
	static sem_t lsem;
	void sendBlockReq(Flow f);
	//void recvBlockReq();
	void tempBlock(std::string ipaddr);
	void remTempBlock(std::string ipaddr);
	void escalate(Flow f);
	bool checkBlacklist();
};

void *recvBlockReq(void * arg);
void *gatewayTaskThread(void *arg);
void *gatewayBlockCleanupThread(void *arg);

bool operator==(const gatewayblock& lhs, const gatewayblock& rhs);

#endif /* GATEWAY_H_ */
