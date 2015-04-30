/*
 * Packet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef PACKET_H_
#define PACKET_H_
#include <stdint.h>

class Packet {
public:
	Packet();
	virtual ~Packet();
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
 * C structure for a packet flow
 * - Contains the ip address of a visited machine
 * - Contains the nonce for that visited machine
 */
struct flow {
    in_addr ip;
    uint64_t nonce;
};

#endif /* PACKET_H_ */
