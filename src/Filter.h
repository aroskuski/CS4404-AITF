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

class Filter {
public:
	Filter();
	virtual ~Filter();
	void startFilterThread(bool host);
private:
	void filterMain();
};

#endif /* FILTER_H_ */
