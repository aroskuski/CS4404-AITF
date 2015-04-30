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

void Hash::hash(unsigned char *in, unsigned char *out){
	HMAC(EVP_md5(), 0, 0, in,0, out, NULL);

}
