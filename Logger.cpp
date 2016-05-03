/*
 * Logger.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: pk
 */

#include "Logger.h"
#include <cstddef>
#include <iostream>
#include <cstdarg>
#include "global.h"
#include <string.h>
#include <errno.h>

Logger* Logger::instance = NULL;
bool Logger::active = false;
FILE* Logger::fd = NULL;

Logger::Logger()
{

}

Logger* Logger::getInstance(void)
{
	if(instance == NULL)
	{
		active = true;
		instance = new Logger();
		fd = fopen("log.txt", "a+");
		if(fd == NULL)
		{
			std::cout << "log.txt open fail, " << strerror(errno) << std::endl;
		}
		return instance;
	}
	else
	{
		return instance;
	}
}

void Logger::log(LogType type, const char * format, ...)
{
    va_list args;
    char info[200];

    memset(info, '\0', sizeof(info));

    va_start(args, format);
    vsnprintf(info, 200, format, args);
    va_end (args);

	if(type == lInfo && active == true)
	{
	  printf("%s\n", info);
	}
	else if(type == lWarning && active == true)
	{
	  printf("%s%s%s\n",YELLOW,  info, RESET);
	}
	else if(type == lError)
	{
	  printf("%s%s%s\n", RED, info, RESET);
	  info[strlen(info)] = '\n';
	  fwrite(info, strlen(info) + 1, sizeof(char), fd);
	}
	else if(type == lFatal)
	{
#warning Skonczyc
		exit(1);
	}
}

