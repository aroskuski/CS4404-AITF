//
//  HostFilter.h
//  Acquiring IP Address
//
//  Created by Alexander W. Witt on 4/28/15.
//  Copyright (c) 2015 ___Alexander W. Witt & Andrew J. Roskuski___. All rights reserved.
//

#ifndef __Acquiring_IP_Address__HostFilter__
#define __Acquiring_IP_Address__HostFilter__

#include <iostream>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/ip.h>
#include <netinet/in.h>
#include <linux/netfilter.h>
#include <linux/types.h>
#include <libnetfilter_queue/libnetfilter_queue.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <string.h>
#include <pthread.h>
#include "Host.h"
#include "Packet.h"
#include "HostPolicyModule.h"


#define FILTERING_REQUEST 1
#define COUNTER_CONNECTION_INITIATION 2
#define COUNTER_CONNECTION_RESPONSE 3

/**
 * The HostFilter class is a C++ class that is used for
 * performing the following tasks on a host machine:
 * (1) Providing flow information from received packets
 * (2) Checking against a shadow table to drop and report non-compliant flows
 * (3) Detecting and report AITF filtering requests issued to the host
 * @version 1.0
 */
class HostFilter {
public:
    /**
     * Constructor for the HostFilter class.
     */
    HostFilter();
    
    /**
     * Destructor for the HostFilter class.
     */
    virtual ~HostFilter();
    
    /**
     * This method is used spawning a thread for carrying out the
     * three responsibilities listed for the HostFilter class.
     */
    void startFilterThread(Host* host);
};

/**
 * This is the main function used by each worker thread that is spawned
 * when the startFilterThread() method of the HostFilter class is used.
 * @return 0 upon success, 1 upon failure.
 */
void* hostFilterMain(void* arg);

/**
 * This is a callback function that is used by the netfilter module
 * whenever a packet is intercepted and read from the netfilter queue.
 * @param qh a pointer to the handle for the nfq queue
 * @param msg a pointer to an nfgenmsg structure
 * @param pkt a pointer to the packet retrieved from the nfq queue
 * @param cbData a pointer to the data of the callback function
 * @return 0 upon success
 */
static int cb(struct nfq_q_handle *qh, struct nfgenmsg *msg, struct nfq_data *pkt, void *cbData);

#endif /* defined(__Acquiring_IP_Address__HostFilter__) */
