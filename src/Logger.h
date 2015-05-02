/*
 * Logger.h
 *
 *  Created on: Apr 11, 2015
 *      Author: andrew
 */

#ifndef __LOGGER_H__
#define __LOGGER_H__

#include <string>
#include <iostream>
#include <fstream>
#include <pthread.h>
#include <semaphore.h>
#include <ctime>

class Logger {
public:
	Logger();
	static void writeToLog(std::string s);
	static void initLog(std::string outfile);
	static void closeLog();
	virtual ~Logger();
private:
	static std::ofstream *outfile;
	static sem_t sem;
};

#endif /* __LOGGER_H__ */
