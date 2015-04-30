/*
 * HostPolicyModule.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef HOSTPOLICYMODULE_H_
#define HOSTPOLICYMODULE_H_
#include "Flow.h"

class HostPolicyModule {
public:
	HostPolicyModule();
	virtual bool checkHighFlowPolicy(Flow f);
	virtual ~HostPolicyModule();
};

#endif /* HOSTPOLICYMODULE_H_ */
