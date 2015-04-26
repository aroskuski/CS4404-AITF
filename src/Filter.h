/*
 * Filter.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <stdint.h>
#include <sys/types.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include "Host.h"
#include "Gateway.h"
#include "ShadowTable.h"

class Filter {
public:
	Filter(ShadowTable &st);
	virtual ~Filter();
	void startFilterThread(bool host);
private:
	void filterMain();
	//Host &h;
	//Gateway &g;
	//ShadowTable &st;
};

#endif /* FILTER_H_ */
