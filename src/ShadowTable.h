/*
 * ShadowTable.h
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#ifndef __SHADOWTABLE_H__
#define __SHADOWTABLE_H__

#include <sys/time.h>
#include <list>
#include <semaphore.h>
#include "Flow.h"

struct ShadowEntry {
	unsigned char ipaddr[4];
	bool active;
	int ttl;
	timespec timeAdded;
	int esccount;//starts at 0
	Flow f;
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
	sem_t sem;
};

bool operator==(const ShadowEntry& lhs, const ShadowEntry& rhs);


#endif /* __SHADOWTABLE_H__ */
