//
//  GatewayFilter.h
//  
//
//  Created by Alexander W. Witt on 4/29/15.
//
//

#ifndef ____GatewayFilter__
#define ____GatewayFilter__

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
#include "Packet.h"

#define FILTERING_REQUEST 1
#define COUNTER_CONNECTION_INITIATION 2
#define COUNTER_CONNECTION_RESPONSE 3

/**
 * The GatewayFilter class is a C++ class that is used for
 * performing the following tasks on a gateway machine:
 * (1) removing the route-record shim if a packet is destined for a legacy host that is next
 * (2) adding a route-record shim to packets that do not have a route record shim
 * (3) checking for filtering responses and reporting those along with a flows to a policy module
 * (4) checking for filtering requests and reporting those along with a flow to a policy module
 * (5) checking to determine whether or not a received packet flow is in the shadow table & reporting
 * (6) checking for filtering initiations and reporting along with flow to a policy module
 * @version 1.0
 */
class GatewayFilter {
public:
    /**
     * Constructor for the GatewayFilter class.
     */
    GatewayFilter();
    
    /**
     * Destructor for the GatewayFilter class.
     */
    virtual ~GatewayFilter();
    
    /**
     * This method is used spawning a thread for carrying out the
     * six responsibilities listed for the GatewayFilter class.
     */
    void startFilterThread(ShadowTable* s);
};

/**
 * This is the main function used by each worker thread that is spawned
 * when the startFilterthread() method of the GatewayFilter class is used.
 * @return 0 upon success, 1 upon failure.
 */
void* gatewayFilterMain(void* arg);

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

#endif /* defined(____GatewayFilter__) */
