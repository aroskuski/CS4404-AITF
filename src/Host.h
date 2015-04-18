/*
 * Host.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef HOST_H_
#define HOST_H_
#include "Flow.h"

class Host {
public:
	Host();
	virtual ~Host();
private:
	void sendBlockReq(Flow f);
	void honorBlockReq();

};

#endif /* HOST_H_ */
