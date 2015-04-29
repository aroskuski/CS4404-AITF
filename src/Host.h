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
#include "Flow.h"
#include "Filter.h"
#include "ShadowTable.h"



class Host {
public:
	Host();
	virtual ~Host();
	void sendMessage(Flow f);
	ShadowTable st;
	void sendBlockReq(Flow f);
	void honorBlockReq(std::string dest_addr);
	static int s;

};

void *hostTaskThread(void *arg);

#endif /* HOST_H_ */
