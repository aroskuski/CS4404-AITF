/*
 * ShadowTable.cpp
 *
 *  Created on: Apr 10, 2015
 *      Author: andrew
 */

#include "ShadowTable.h"

bool operator==(const ShadowEntry& lhs, const ShadowEntry& rhs){

	bool result = lhs.ipaddr[0] == rhs.ipaddr[0] && lhs.ttl == rhs.ttl && lhs.ipaddr[1] == rhs.ipaddr[1] && lhs.ipaddr[2] == rhs.ipaddr[2] && lhs.ipaddr[3] == rhs.ipaddr[3] && lhs.esccount == rhs.esccount && lhs.active && rhs.active;
	return result && lhs.f == rhs.f;
}

ShadowTable::ShadowTable() {
	// TODO Auto-generated constructor stub
	hostList = std::list<ShadowEntry>();
	sem_init(&sem, 0, 1);
}

ShadowTable::~ShadowTable() {
	// TODO Auto-generated destructor stub
}

void ShadowTable::addFlow(Flow f){
	sem_wait(&sem);
	ShadowEntry newse = ShadowEntry();
	for(int i = 0; i < 4; i++){
		newse.ipaddr[i] = f.getAttackHost().ipaddr[i];
	}

	newse.esccount = 0;
	newse.f = f;
	newse.active = false;
	newse.ttl = 30;
	clock_gettime(CLOCK_MONOTONIC, &newse.timeAdded);
	hostList.push_back(newse);
	sem_post(&sem);
}

void ShadowTable::remFlow(Flow f){
	sem_wait(&sem);
	ShadowEntry se;
	for(std::list<ShadowEntry>::iterator it = hostList.begin(); it != hostList.end(); it++){
		if (it->f == f){
			se = *it;
		}
	}
	hostList.remove(se);
	sem_post(&sem);
}

bool ShadowTable::containsHost(unsigned char *h){
	sem_wait(&sem);
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	for(std::list<ShadowEntry>::iterator it = hostList.begin(); it != hostList.end(); it++){
		if (it->ipaddr[0] == h[0] && it->ipaddr[1] == h[1] && it->ipaddr[2] == h[2] && it->ipaddr[3] == h[3] && (ts.tv_sec < it->timeAdded.tv_sec + it->ttl)){
			sem_post(&sem);
			return true;
		}
	}
	sem_post(&sem);
	return false;

}

bool ShadowTable::containsFlow(Flow f){
	sem_wait(&sem);
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	for(std::list<ShadowEntry>::iterator it = hostList.begin(); it != hostList.end(); it++){
		if (it->f == f && (ts.tv_sec < it->timeAdded.tv_sec + it->ttl)){
			sem_post(&sem);
			return true;
		}
	}
	sem_post(&sem);
	return false;

}

void ShadowTable::clean(){
	sem_wait(&sem);
	std::list<ShadowEntry> toRemove = std::list<ShadowEntry>();
	timespec ts;
	clock_gettime(CLOCK_MONOTONIC, &ts);
	for(std::list<ShadowEntry>::iterator it = hostList.begin(); it != hostList.end(); it++){

		if (ts.tv_sec > it->timeAdded.tv_sec + it->ttl){
			toRemove.push_back(*it);
		}
	}
	for(std::list<ShadowEntry>::iterator it = toRemove.begin(); it != toRemove.end(); it++){
		hostList.remove(*it);
	}
	sem_post(&sem);
}

ShadowEntry *ShadowTable::getshadowEntry(Flow f){
	sem_wait(&sem);
	for(std::list<ShadowEntry>::iterator it = hostList.begin(); it != hostList.end(); it++){
		if (it->f == f){
			sem_post(&sem);
			return &(*it);
		}
	}
	sem_post(&sem);
	return NULL;
}
