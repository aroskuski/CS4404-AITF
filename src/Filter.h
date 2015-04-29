/*
 * Filter.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef FILTER_H_
#define FILTER_H_
#include <iostream>
#include <stdint.h>
#include <stdlib.h>
#include <sys/types.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include "Host.h"
#include "Gateway.h"
#include "ShadowTable.h"

/**
 * The Filter class is a C++ class that is used for
 * monitoring and manipulating all packets that are
 * either arriving at or transiting through the machine
 * on which a Filter instance is deployed. Particular
 * responsibilities include sending and reporting all packet
 * flow information, adding route record information to
 * packets, and rejecting certain packets.
 * @version 1.0
 */
class Filter {

public:
	/**
	 * Constructor for the Filter class.
	 * @param st a pointer to the shadow table for this Filter instance.
	 */
	Filter(ShadowTable &st);

	/**
	 * Destructor for the Filter class.
	 */
	virtual ~Filter();

	/**
	 * This method is used to initiate the thread
	 * used for monitoring and modifying packets.
	 * @param host whether or not the machine is a host
	 */
	void startFilterThread(bool host);

private:
	/**
	 * This method is essentially the method run by the
	 * thread used for filtering in the Filter class.
	 * @param host whether or not the machine is a host
	 */
	void filterMain(bool host);
	//Host &h;
	//Gateway &g;
	//ShadowTable &st;
};

/**
 * This is a callback function that is used by netfilter
 * whenever a packet is intercepted and read from the netfilter queue.
 * @param qh a pointer to the handle for the nfq queue
 * @param msg a pointer to an nfgenmsg structure
 * @param pkt a pointer to the packet retrieved from the nfq queue
 * @param cbData a pointer to the data of the callback function
 * @return 0 upon success
 */
static int callback(struct nfq_q_handle *qh, struct nfgenmsg *msg, nfq_data *pkt, void *cbData);

#endif /* FILTER_H_ */
