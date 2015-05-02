/*
 * Logger.cpp
 *
 *  Created on: Apr 11, 2015
 *      Author: andrew
 */

#include "Logger.h"

std::ofstream *Logger::outfile = new std::ofstream();
sem_t Logger::sem;

Logger::Logger() {
	// TODO Auto-generated constructor stub

}

Logger::~Logger() {
	// TODO Auto-generated destructor stub
}

void Logger::writeToLog(std::string s){
	//ofstream &out = *(Logger::outfile);
	time_t t = time(0);
	struct tm * timeinfo;
	timeinfo = localtime(&t);
	sem_wait(&sem);
	*(Logger::outfile) << asctime(timeinfo) << ": " << s << '\n';
	sem_post(&sem);
}


void Logger::initLog(std::string out){
	//Logger::outfile = new ofstream();
	sem_init(&sem, 0, 1);
	Logger::outfile->open(out.c_str());
}

void Logger::closeLog(){
	sem_wait(&sem);
	sem_destroy(&sem);
	Logger::outfile->close();
	delete Logger::outfile;
}
