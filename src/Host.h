/*
 * Host.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef __HOST_H__
#define __HOST_H__

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <queue>
#include <list>
#include "Flow.h"
#include "Packet.h"
#include "ShadowTable.h"

#define PACKET_PAYLOAD_SIZE 1500
#define PORT "1025"

struct hostblock{
	std::string ipaddr;
	int ttl;
};

class Host {
public:
	Host();
	virtual ~Host();
	void sendMessage(Flow f);
	ShadowTable st;
	void sendBlockReq(Flow f);
	void honorBlockReq(std::string dest_addr);
	static int s;
	//static pthread_mutex_t m;
	static sem_t sem;
	static sem_t qsem;
	static std::queue<Flow> q;
	static std::list<hostblock> blocklist;
	static sem_t lsem;
};

void *hostTaskThread(void *arg);
void *hostRecvThread(void *arg);
void *hostBlockCleanupThread(void *arg);

#endif /* __HOST_H__ */
