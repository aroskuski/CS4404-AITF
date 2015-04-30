/*
 * Packet.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef PACKET_H_
#define PACKET_H_

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

#endif /* PACKET_H_ */
