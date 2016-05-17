/*
 * FileStorage.h
 *
 *  Created on: Apr 8, 2016
 *      Author: pk
 */

#ifndef FILESTORAGE_H_
#define FILESTORAGE_H_

#include <list>
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "SystemCommands.h"
#include "IStorage.h"
#include "Parser.h"
#include "File.h"
#include "Buffer.h"
#include "pthread.h"
#include "FileHandler.h"

class FileStorage : public IStorage
{
public:
	FileStorage(Parser const * const parser, SystemCommands const * const _cmd, int storePathIndex);
	~FileStorage();
	int init(void);
	bool isProperName(std::string);
	void add(uint8_t *pData, uint16_t len);
	void update(void);
private:
	std::list<File> fileList;
	uint64_t totalSize;
	uint64_t lastFileSize;
	Parser const * const pars;
	SystemCommands const * const cmd;
	Buffer *buffer;
	pthread_mutex_t buffferLock;
	FileHandler *fileHandler;
	std::string todaysDate;
	int storePathIndex;

	bool listFiles(void);
	bool isGrater(std::string name, std::string fromList);
	void addFiles(std::string name, uint64_t size);
	void removeFilesDueToSize(void);
	void removeFilesDueToExpirationDate(void);
	void printFileList(void);
	bool mountDrive(void);
	void addCurrentFile(int len);
	void flush(void);
	bool newFileRequired(void);
	bool checkIfOldFile(std::string name);
	bool createFileDirectory(void);
};

#endif /* FILESTORAGE_H_ */
