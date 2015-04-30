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

void Hash::hash(unsigned char *in, int insize, unsigned char *out){
	const char key[33] = "EVERYONE HAPPY SIMPLE CONNECTION";
	HMAC(EVP_md5(),(void *) key, 32, in, insize, out, NULL);

}
