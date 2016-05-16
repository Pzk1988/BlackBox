/*
 * FileHandler.cpp
 *
 *  Created on: Apr 21, 2016
 *      Author: pk
 */

#include "FileHandler.h"

FileHandler::FileHandler(SystemCommands const * const systemCommands): cmd(systemCommands)
{
	this->fd = NULL;
}

FileHandler::~FileHandler()
{
	if(fd != NULL)
	{
		fflush(fd);
		fclose(fd);
		fd = NULL;
	}
}

void FileHandler::open(std::string name)
{
	close();
	
	fd = fopen(name.c_str(), "a+");
	if(fd == NULL)
	{
		Logger::getInstance()->log(lFatal, "Can`t open file %s, %s", name.c_str(), strerror(errno));
		cmd->umount();
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Open file %s", name.c_str());
	}
}

void FileHandler::write(uint8_t *pData, int len)
{
	if(fwrite(pData, sizeof(uint8_t), len, fd) != (unsigned int)len)
	{
		Logger::getInstance()->log(lFatal, "Can`t write to file %s", strerror(errno));
		cmd->umount();
	}
}

void FileHandler::close(void)
{
	if(fd != NULL)
	{
		fflush(fd);
		fclose(fd);
		fd = NULL;
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Close file");
	}
}

void FileHandler::flush(void)
{
	if(fd != NULL)
	{
		fflush(fd);
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Flush file");
	}
}
