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
#include <cstring>
#include "Flow.h"
#include "Filter.h"


class Host {
public:
	Host();
	virtual ~Host();
private:
	void sendBlockReq(Flow f);
	void honorBlockReq();

};

#endif /* HOST_H_ */
