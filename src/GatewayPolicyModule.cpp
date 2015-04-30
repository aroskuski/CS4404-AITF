/*
 * GatewayPolicyModule.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "GatewayPolicyModule.h"

GatewayPolicyModule::GatewayPolicyModule() {
	// TODO Auto-generated constructor stub

}

GatewayPolicyModule::~GatewayPolicyModule() {
	// TODO Auto-generated destructor stub
}

virtual bool GatewayPolicyModule::checkEscPolicy(Flow f){
	return true;
}
