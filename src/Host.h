/*
 * Host.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef HOST_H_
#define HOST_H_
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <cstring>
#include <string>
#include <pthread.h>
#include <semaphore.h>
#include <queue>
#include "Flow.h"
#include "HostFilter.h"
#include "HostFilter.h"
#include "ShadowTable.h"

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


#endif /* HOST_H_ */
