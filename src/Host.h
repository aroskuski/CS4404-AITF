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
#include "Flow.h"
#include "Filter.h"
#include "ShadowTable.h"


class Host {
public:
	Host();
	virtual ~Host();
	ShadowTable st;
private:
	void sendBlockReq(Flow f);
	void honorBlockReq(std::string dest_addr);

};

#endif /* HOST_H_ */
