/*
 * Packet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef __PACKET_H__
#define __PACKET_H__
#include <stdint.h>
#include <netinet/ip.h>
#include "Flow.h"

#define FILTERING_REQUEST 1
#define COUNTER_CONNECTION_INITIATION 2
#define COUNTER_CONNECTION_RESPONSE 3

uint16_t checksum(void* vdata, size_t length);

class Packet {
public:
	Packet();
	virtual ~Packet();
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
 * C structure for the AITF application layer protocol header
 * -
 */
struct AITFHeader {
    uint8_t commandFlags;
    struct flow pktFlow[6];
    uint64_t nonce;
    uint16_t flowSize;
    uint16_t payloadSize;
};

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
 * C structure for a route record
 * - Contains a packet flow for six flow entries
 * - Contains the current position for adding another flow entry
 * - Contains the maximum length for the packet flow
 */
struct routeRecord {
    struct flow pktFlow[10];
    uint16_t position;
    uint16_t length;
    uint8_t protocol;
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
    struct routeRecord rr;
    struct AITFHeader aitfHeader;
    char payload[1500];
};

struct RRPacket {
    struct ip ipHeader;
    struct routeRecord routeRecord;
    struct tcp tcpHeader;
    struct AITFHeader aitfHeader;
    char payload[1500];
};

#endif /* __PACKET_H__ */
