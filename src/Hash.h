/*
 * Hash.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef HASH_H_
#define HASH_H_

class Hash {
public:
	Hash();
	static void hash(unsigned char *in, unsigned char *out);
	virtual ~Hash();
};

#endif /* HASH_H_ */
