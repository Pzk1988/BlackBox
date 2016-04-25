/*
 * Logger.h
 *
 *  Created on: Apr 5, 2016
 *      Author: pk
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <stdio.h>

typedef enum logType {lFatal, lWarning, lError, lMinor, lInfo}LogType;

class Logger
{
public:
	static Logger* getInstance(void);
	void log(LogType type, const char * format, ...);

private:
	Logger();
	static Logger* instance;
	static bool active;
	static FILE *fd;
};

#endif /* LOGGER_H_ */

