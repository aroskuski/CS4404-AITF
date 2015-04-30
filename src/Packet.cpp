/*
 * Packet.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Packet.h"

uint16_t checksum(void* vdata, size_t length) {
	char* data = (char*) vdata;
	uint32_t acc = 0xffff;
	for (size_t i = 0; i + 1 < length; i += 2) {
		uint16_t word;
		memcpy(&word, data + i, 2);
		acc += ntohs(word);
		if (acc > 0xffff) {
			acc -= 0xffff;
		}
	}
	if (length & 1) {
		uint16_t word = 0;
		memcpy(&word, data + length - 1, 1);
		acc += ntohs(word);
		if (acc > 0xffff) {
			acc -= 0xffff;
		}
	}
	return htons(~acc);
}

Packet::Packet() {
	// TODO Auto-generated constructor stub

}

Packet::~Packet() {
	// TODO Auto-generated destructor stub
}
