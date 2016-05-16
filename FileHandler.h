/*
 * FileHandler.h
 *
 *  Created on: Apr 21, 2016
 *      Author: pk
 */

#ifndef FILEHANDLER_H_
#define FILEHANDLER_H_
#include <iostream>
#include <stdio.h>
#include <errno.h>
#include "Logger.h"
#include <string.h>
#include <inttypes.h>
#include "SystemCommands.h"

class FileHandler {
public:
	FileHandler(SystemCommands const * const systemCommands);
	virtual ~FileHandler();
	virtual void open(std::string name);
	virtual void write(uint8_t *pData, int len);
	virtual void close(void);
	virtual void flush(void);
protected:
	FILE *fd;
	SystemCommands const * const cmd;
};

#endif /* FILEHANDLER_H_ */
