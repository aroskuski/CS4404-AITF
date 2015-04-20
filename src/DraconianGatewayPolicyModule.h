/*
 * DraconianGatewayPolicyModule.h
 *
 *  Created on: Apr 20, 2015
 *      Author: andrew
 */

#ifndef DRACONIANGATEWAYPOLICYMODULE_H_
#define DRACONIANGATEWAYPOLICYMODULE_H_

#include "GatewayPolicyModule.h"

namespace policy {

class DraconianGatewayPolicyModule: public GatewayPolicyModule {
public:
	DraconianGatewayPolicyModule();
	bool checkEscPolicy(Flow f);
	virtual ~DraconianGatewayPolicyModule();
};

} /* namespace policy */

#endif /* DRACONIANGATEWAYPOLICYMODULE_H_ */
