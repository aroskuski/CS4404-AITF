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
    void startFilterThread();
};

/**
 * This is the main function used by each worker thread that is spawned
 * when the startFilterThread() method of the HostFilter class is used.
 * @return 0 upon success, 1 upon failure.
 */
int hostFilterMain();

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

// PACKET RELATED DATA STRUCTURES

/**
 * C structure for a typical TCP header
 */
struct tcp {
    uint16_t sourcePort;
    uint16_t destPort;
    uint32_t seqNum;
    uint32_t ackNum;
    uint16_t info;
    uint16_t window;
    uint16_t checksum;
    uint16_t urg;
};

/**
 * C structure for a typical TCP packet with an IP header
 * - Contains the IPv4 header for the packet
 * - Contains the TCP header for the packet
 * - Contains the payload for the packet
 */
struct tcpPacket {
    struct ip ipHeader;
    struct tcp tcpHeader;
    char payload[1500];
};

/**
 * C structure for any packet with an IPv4 header
 * - Contains the IPv4 header for the packet
 * - Contains the payload for the packet
 */
struct ipPacket {
    struct ip ipHeader;
    char payload[1500];
};

/**
 * C structure for a packet flow
 * - Contains the ip address of a visited machine
 * - Contains the nonce for that visited machine
 */
struct flow {
    in_addr ip;
    uint64_t nonce;
};

/**
 * C structure for a route record
 * - Contains a packet flow for six flow entries
 * - Contains the current position for adding another flow entry
 * - Contains the maximum length for the packet flow
 */
struct routeRecord {
    struct flow pktFlow[6];
    uint16_t position;
    uint16_t length;
};

/**
 * C structure for the AITF application layer protocol header
 * -
 */
struct AITFHeader {
    uint8_t commandFlags;
    struct flow pktFlow[6];
    uint64_t nonce;
    uint16_t payloadSize;
};

/**
 * C structure for an AITF packet
 * - Contains the IPv4 header for the packet
 * - Contains the route record shim for the packet
 * - Contains the UDP header for the packet
 * - Contains the AITF application layer protocol header
 */
struct AITFPacket {
    struct ip ipHeader;
    struct tcp tcpHeader;
    struct AITFHeader aitfHeader;
    char payload[1500];
};

#endif /* defined(__Acquiring_IP_Address__HostFilter__) */
