/*
 * HostPolicyModule.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "HostPolicyModule.h"

HostPolicyModule::HostPolicyModule() {
	// TODO Auto-generated constructor stub

}

HostPolicyModule::~HostPolicyModule() {
	// TODO Auto-generated destructor stub
}

virtual bool HostPolicyModule::checkHighFlowPolicy(Flow f){

	return false;
}
