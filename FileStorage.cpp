/*
 * FileStorage.cpp
 *
 *  Created on: Apr 8, 2016
 *      Author: pk
 */

#include "FileStorage.h"
#include "Logger.h"
#include <errno.h>
#include <string.h>
#include <dirent.h>
#include <ctype.h>
#include "TimeCounter.h"
#include "global.h"

FileStorage::FileStorage(Parser const * const parser, SystemCommands const * const _cmd) : pars(parser), cmd(_cmd)
{
	this->buffer = NULL;
	this->sdCardError = false;
	this->totalSize = 0;
	this->lastFileSize = 0;
	pthread_mutex_init(&buffferLock, NULL);
	this->fileHandler = NULL;
}

int FileStorage::init(void)
{
	//Create new buffer
	buffer = new Buffer();

	//Create new file handler
	fileHandler = new FileHandler();

	//Check if mount is necessary
	if(mountDrive() != false)
	{
		this->sdCardError = false;

		if(listFiles() == true)
		{
			//Remove file to meet allowed storage size
			removeFiles();

			//Add current file
			addCurrentFile(0);

			//Open file to write
			fileHandler->open(this->pars->getMountDst() + "/" + fileList.back().getName());

			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		//this->sdCardError = true;
		this->sdCardError = true;
		return -1;
	}
}

FileStorage::~FileStorage()
{
	fileHandler->close();
	std::cout << "FileStorage destructor\n";
}

void FileStorage::operator<< (std::string log)
{

}

bool FileStorage::listFiles(void)
{
	std::string filePath;
	DIR *dir = opendir(this->pars->getMountDst().c_str());
	struct dirent *directory;
	struct stat st;

	if(dir != NULL)
	{
		while((directory = readdir(dir)) != NULL)
		{
			filePath = this->pars->getMountDst() + "/" + directory->d_name;

			stat(filePath.c_str(), &st);

			if(S_ISREG(st.st_mode) == true)
			{
				addFiles(directory->d_name, st.st_blocks * 512);
			}
			else
			{
				Logger::getInstance()->log(lInfo, "'%s' is not a file", directory->d_name);
			}
		}

		std::cout << "Total size " << totalSize << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}

bool FileStorage::isProperName(std::string name)
{
	for(uint8_t i = 0; i < name.length(); i++)
	{
		if(i < 8)
		{
			if(isdigit(name[i]) == false)
			{
				return false;
			}
		}
		else if( i < 9)
		{
			if(name[i] != '_')
			{
				return false;
			}
		}
		else
		{
			if(isdigit(name[i]) == false)
			{
				return false;
			}
		}
	}
	return true;
}

bool FileStorage::isGrater(std::string name, std::string fromList)
{
	//uint8_t len = name.length() > fromList.length() ? fromList.length() : name.length();
	uint16_t nameDesignator = 0;
	uint16_t fromListDesignator = 0;

	for(uint8_t i = 0; i < 8; i++)
	{
		if(name[i] > fromList[i])
		{
			return true;
		}
		else if(name[i] < fromList[i])
		{
			return false;
		}
	}

	if(name.length() > 8)
	{
		nameDesignator = atoi(&name.c_str()[9]);
	}

	if(fromList.length() > 8 )
	{
		fromListDesignator = atoi(&fromList.c_str()[9]);
	}

	if(nameDesignator > fromListDesignator)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void FileStorage::addFiles(std::string name, uint64_t size)
{
	if(isProperName(name) == true)
	{
		if(fileList.empty() != true)
		{
			std::list<File>::iterator it = fileList.begin();
			while(it != fileList.end())
			{
				if(isGrater(name, ((File)(*it)).getName()) == true)
				{
					it++;
					if(it == fileList.end())
					{
						fileList.push_back(File(name, size));
						totalSize += size;
						Logger::getInstance()->log(lInfo, "Add file %s\t[%d] to list", name.c_str(), size);
					}
				}
				else
				{
					fileList.insert(it, File(name, size));
					totalSize += size;
					Logger::getInstance()->log(lInfo, "Add file %s\t[%d] to list", name.c_str(), size);
					break;
				}
			};
		}
		else
		{
			fileList.push_back(File(name, size));
			totalSize += size;
			Logger::getInstance()->log(lInfo, "Add file %s\t[%d] to list", name.c_str(), size);
		}
	}
	else
	{
		Logger::getInstance()->log(lError, "Incorrect file name '%s'", name.c_str());
	}
}

void FileStorage::removeFiles(void)
{
	while(totalSize + lastFileSize > pars->getMaxTotalSize())
	{
		//#error obsluzyc sytuacje w ktorej nie mozna usunac pliku
		File &temp = fileList.front();

		if(cmd->remove(temp.getName()) == 0)
		{
			totalSize -= temp.getSize();
			fileList.pop_front();
		}
		else
		{
			break;
		}
	}
}

void FileStorage::printFileList(void)
{
	std::cout << std::endl;
	for(std::list<File>::iterator it = fileList.begin(); it != fileList.end(); it++)
	{
		std::cout << (*it).getName() << ", size: " << (*it).getSize() << std::endl;
	}
	std::cout << std::endl;
}

bool FileStorage::mountDrive(void)
{
	if(pars->getMountSrc().empty() == false)
	{
		if(cmd->mount(pars->getMountSrc(), pars->getMountDst()) == true)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Mount omitted");
		return true;
	}
}

void FileStorage::addCurrentFile(int len)
{
	char fileName[9];
	time_t t;
	struct tm *tm;
	struct stat st;

	t = time(NULL);
	tm = localtime(&t);
	if (tm == NULL)
	{
        Logger::getInstance()->log(lError, "Unable to fetch current date");
    }

	strftime(fileName, 9, "%Y%m%d", tm);

	if(fileList.size() != 0)
	{
		File &temp = fileList.back();
		if(stat(std::string(pars->getMountDst() + '/' + temp.getName()).c_str(), &st) != 0)
		{
			Logger::getInstance()->log(lWarning, "Unable to read file size");
		}

		if(strncmp(fileName, temp.getName().c_str(), 8) != 0)
		{
			cmd->create(fileName);
			fileList.push_back(File(fileName, 0));
			this->lastFileSize = 0;
		}
		else if(st.st_size + len > pars->getMaxFileSize())
		{
			if(temp.getName().length() > 8)
			{
				int i = atoi(&temp.getName().c_str()[9]);
				char tab[20];
				snprintf(tab, 20, "%s_%d", fileName, i + 1);
				cmd->create(tab);
				fileList.push_back(File(tab, 0));
				this->lastFileSize = 0;
			}
			else
			{
				cmd->create(temp.getName() + "_1");
				fileList.push_back(File(temp.getName() + "_1", 0));
				this->lastFileSize = 0;
			}
		}
		else
		{
			std::cout << "Current file is correct '" << temp.getName() << "', size: " << st.st_size << std::endl;
			this->lastFileSize = st.st_size;

		}
	}
	else
	{
		cmd->create(fileName);
		fileList.push_back(File(fileName, 0));
		this->lastFileSize = 0;
	}
}

void FileStorage::update(void)
{
	pthread_mutex_lock(&buffferLock);
	if(buffer->isEmpty() != true)
	{
		if(buffer->delayTime() > (uint64_t)pars->getPacketInterval())
		{
			//Flush files from buffer to file
			flush();
		}
	}
	pthread_mutex_unlock(&buffferLock);
}

void FileStorage::add(uint8_t *pData, uint16_t len)
{
	pthread_mutex_lock(&buffferLock);

	//Check if buffer is full
	if(buffer->isFull() == true)
	{
		//Flush files from buffer to file
		flush();
	}

	//Add msg to buffer
	buffer->add(pData, len);
	pthread_mutex_unlock(&buffferLock);
}

void FileStorage::flush(void)
{
	int len;
	uint8_t data[MAX_MSG_LEN];
	int i = 0;

	//Remove old files
	removeFiles();

	//Store all logs from buffer
	while(buffer->get(data, &len, i) == true)
	{
		//Check if log can fit to current file or if new file due to next day have to be created
		if(((this->lastFileSize + len) > (uint64_t)pars->getMaxFileSize()) || (newFileRequired() == true))
		{
			//Close current file
			fileHandler->close();

			//Update size of file that is being closed
			struct stat st;
			stat(std::string(this->pars->getMountDst() + "/" + fileList.back().getName()).c_str(), &st);
			fileList.back().setSize(st.st_blocks * 512);
			this->totalSize += st.st_blocks * 512;

			//Create new file
			addCurrentFile(len);

			//Open new file
			fileHandler->open(this->pars->getMountDst() + "/" + fileList.back().getName());

			//Set local size of new file to 0
			this->lastFileSize = 0;

			//Send info
			Logger::getInstance()->log(lInfo, "New file %s", fileList.back().getName().c_str());
		}

		//Add size of log to total size of file
		this->lastFileSize += len;

		//Write current log to file
		fileHandler->write(data, len);
	}

	Logger::getInstance()->log(lInfo, "Flush");

	//Flush file
	fileHandler->flush();
}

bool FileStorage::newFileRequired(void)
{
	char fileName[9];
	time_t t;
	struct tm *tm;

	t = time(NULL);
	tm = localtime(&t);
	if (tm == NULL)
	{
        Logger::getInstance()->log(lError, "Unable to fetch current date");
    }

	strftime(fileName, 9, "%Y%m%d", tm);

	if(strncmp(fileName, fileList.back().getName().c_str(), 8) != 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}



