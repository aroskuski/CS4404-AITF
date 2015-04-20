/*
 * Logger.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: andrew
 */

#include "Logger.h"

ofstream *Logger::outfile = new ofstream();

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::writeToLog(string s){
	//ofstream &out = *(Logger::outfile);
	*(Logger::outfile) << s << '\n';
}


void Logger::initLog(string out){
	//Logger::outfile = new ofstream();
	Logger::outfile->open(out.c_str());
}

void Logger::closeLog(){
	Logger::outfile->close();
	delete Logger::outfile;
}
