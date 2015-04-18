/*
 * ShadowTable.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef SHADOWTABLE_H_
#define SHADOWTABLE_H_
#include <sys/time.h>
#include <list>

struct ShadowEntry {
	unsigned char ipaddr[4];
	bool active;
	int ttl;
	timespec timeAdded;
};

class ShadowTable {
public:
	ShadowTable();
	virtual ~ShadowTable();
	void addHost(unsigned char h[4]);
	void remHost(unsigned char h[4]);
	bool containsHost(unsigned char h[4]);
	void clean();
private:
	std::list<ShadowEntry> hostList;
};

#endif /* SHADOWTABLE_H_ */
