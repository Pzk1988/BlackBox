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

class FileHandler {
public:
	FileHandler();
	virtual ~FileHandler();
	void open(std::string name);
	void write(uint8_t *pData, int len);
	void close(void);
	void flush(void);
private:
	FILE *fd;
};

#endif /* FILEHANDLER_H_ */
