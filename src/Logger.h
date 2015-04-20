/*
 * Logger.h
 *
 *  Created on: Apr 11, 2015
 *      Author: andrew
 */

#ifndef LOGGER_H_
#define LOGGER_H_
#include <string>
#include <iostream>
#include <fstream>
#include <pthread.h>
using namespace std;
class Logger {
public:
	Logger();
	static void writeToLog(string s);
	static void initLog(string outfile);
	static void closeLog();
	virtual ~Logger();
private:
	static ofstream *outfile;
};

#endif /* LOGGER_H_ */
