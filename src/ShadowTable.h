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
#include "Flow.h"

struct ShadowEntry {
	unsigned char ipaddr[4];
	bool active;
	int ttl;
	timespec timeAdded;
	int esccount;//starts at 0
};

class ShadowTable {
public:
	ShadowTable();
	virtual ~ShadowTable();
	void addFlow(Flow f);
	void remFlow(Flow f);
	bool containsHost(unsigned char *h);
	bool containsFlow(Flow f);
	ShadowEntry *getshadowEntry(Flow f);
	void clean();
private:
	std::list<ShadowEntry> hostList;
};

#endif /* SHADOWTABLE_H_ */
