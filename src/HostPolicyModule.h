/*
 * HostPolicyModule.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef __HOSTPOLICYMODULE_H__
#define __HOSTPOLICYMODULE_H__
#include "Flow.h"

class HostPolicyModule {
public:
	HostPolicyModule();
	virtual bool checkHighFlowPolicy(Flow f);
	virtual ~HostPolicyModule();
};

#endif /* __HOSTPOLICYMODULE_H__ */
