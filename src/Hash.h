/*
 * Hash.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef HASH_H_
#define HASH_H_
#include <openssl/hmac.h>
#include <openssl/evp.h>

class Hash {
public:
	Hash();
	static unsigned char *hash(unsigned char *in, int insize, unsigned int *outlen);
	virtual ~Hash();
};

#endif /* HASH_H_ */
