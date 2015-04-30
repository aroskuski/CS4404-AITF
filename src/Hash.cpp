/*
 * Hash.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "Hash.h"


Hash::Hash() {
	// TODO Auto-generated constructor stub

}

Hash::~Hash() {
	// TODO Auto-generated destructor stub
}

unsigned char* Hash::hash(unsigned char *in, int insize, unsigned int *outlen){
	const char key[33] = "EVERYONE HAPPY SIMPLE CONNECTION";
	return HMAC(EVP_md5(),(void *) key, 32, in, insize, NULL, outlen);

}
