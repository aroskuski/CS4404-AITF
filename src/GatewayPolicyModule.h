/*
 * GatewayPolicyModule.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef GATEWAYPOLICYMODULE_H_
#define GATEWAYPOLICYMODULE_H_
#include "Flow.h"

class GatewayPolicyModule {
public:
	GatewayPolicyModule();
	virtual bool checkEscPolicy(Flow f);
	virtual ~GatewayPolicyModule();
};

#endif /* GATEWAYPOLICYMODULE_H_ */
