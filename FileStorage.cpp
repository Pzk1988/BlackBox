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
	this->fd = NULL;
	pthread_mutex_init(&buffferLock, NULL);
}

int FileStorage::init(void)
{
	//Create new buffer
	buffer = new Buffer();

	if(mountDrive() != false)
	{
		this->sdCardError = false;

		if(listFiles() == true)
		{
			//printFileList();
			//Remove file to meet allowed storage size
			removeFiles();

			//Add current file
			addCurrentFile();

			//Open file to write
			fd = fopen(std::string(this->pars->getMountDst() + "/" + fileList.back().getName()).c_str(), "a+");
			if(fd == NULL)
			{
				Logger::getInstance()->log(lError, "Can`t open file %s, %s", std::string(this->pars->getMountDst() + "/" + fileList.back().getName()).c_str(), strerror(errno));
			}

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
	flush();
	fclose(fd);
	std::cout << "FileStorage destructor\n";
}

void FileStorage::operator<< (std::string log)
{

}

void FileStorage::add(uint8_t *pData, uint16_t len)
{
	pthread_mutex_lock(&buffferLock);
	if(buffer->isFull() == true)
	{
		flush();
	}

	buffer->add(pData, len);
	std::cout << "Add " << len << std::endl;
	pthread_mutex_unlock(&buffferLock);
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
	while(totalSize > pars->getMaxTotalSize())
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
	for(std::list<File>::iterator it = fileList.begin(); it != fileList.end(); it++)
	{
		std::cout << (*it).getName() << ", size: " << (*it).getSize() << std::endl;
	}
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

void FileStorage::addCurrentFile(void)
{
	char fileName[9];
	time_t t;
	struct tm *tm;

	File &temp = fileList.back();

	t = time(NULL);
	tm = localtime(&t);
	if (tm == NULL)
	{
        Logger::getInstance()->log(lError, "Unable to fetch current date");
    }

	strftime(fileName, 9, "%Y%m%d", tm);

	if(strncmp(fileName, temp.getName().c_str(), 8) != 0)
	{
		cmd->create(fileName);
	}
	else if(temp.getSize() > pars->getMaxFileSize())
	{
		if(temp.getName().length() > 8)
		{
			int i = atoi(&temp.getName().c_str()[9]);
			char tab[20];
			snprintf(tab, 20, "%s_%d", fileName, i + 1);
			cmd->create(tab);
			fileList.push_back(File(tab, 0));
		}
		else
		{
			cmd->create(temp.getName() + "_1");
			fileList.push_back(File(temp.getName() + "_1", 0));
		}
	}
	else
	{
		std::cout << "Current file is correct '" << temp.getName() << std::endl;
	}
}

void FileStorage::update(void)
{
	pthread_mutex_lock(&buffferLock);
	if(buffer->isEmpty() != true)
	{
		if(buffer->delayTime() > (uint64_t)pars->getPacketInterval())
		{
			flush();
		}
	}
	pthread_mutex_unlock(&buffferLock);
}

void FileStorage::flush(void)
{
	int len;
	uint8_t data[MAX_MSG_LEN];
	int i = 0;

	while(buffer->get(data, &len, i) == true)
	{
		i++;
		std::cout << "2tu " << len << "\n";
		data[len] = '\0';
		printf("%s\n", data);
		if(fwrite(data, sizeof(uint8_t), len, fd) != (unsigned int)len)
		{
			Logger::getInstance()->log(lError, "Can`t write to file %s", strerror(errno));
		}
	}
	fflush(fd);
}
