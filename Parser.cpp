/*
 * Parser.cpp
 *
 *  Created on: Apr 4, 2016
 *      Author: pk
 */

#include "Parser.h"
#include <fstream>
#include <iostream>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <vector>
#include <sys/stat.h>
#include "Logger.h"
#include "global.h"

Parser::Parser(std::string path)
{
    this->line = NULL;
    this->path = path;			//Direction where to store log files
    this->daysToSave = 30;		//Time to store log files
    this->debug = false;
    this->ip = "";				//
    this->maxFileSize = 15;		//Max file size in MB
    this->maxTotalSize = 4096;	//Max total size of all log file on drive in MB
    this->packetInterval = 10;	//Amount of packages buffered
    this->timeInterval = 60;	//Time after buffers are stored into hard drive
    this->mountSrc = "";		//Mount point source
    this->mountDst = "";		//Mount point destination

    this->parsError = false;
    this->portList = new std::vector<uint16_t>();
    this->lineCounter = 0;
    this->keyword = NULL;
    this->value = NULL;
}

Parser::~Parser()
{
}

bool Parser::read()
{
    FILE* fd;
    size_t len = 0;
    char *pEnd;

    fd = fopen(path.c_str(), "r");

    if(fd == NULL)
    {
	Logger::getInstance()->log(lError, "Failed to open file %s", path.c_str());
        return false;
    }
    while(getline(&line, &len, fd) != -1)
    {
        lineCounter++;

	if(strlen(line) > 2)
	{
	    if(strncmp(line, "//", 2) != 0 && line[0] != '#')
	    {
	    if(parsLine() == true)
	    {
		if(strlen(keyword) > 0 && strlen(value) > 0)
		{
		      //if(strncmp(keyword, "//", 2) != 0 && keyword[0] != '#')
		      //{
			if(strcmp(keyword, "ip") == 0)
			{
			    if(checkIp() == true)
			    {
				if(parsIp() == false)
				{
				    Logger::getInstance()->log(lError, "Invalid ip address format in line %d", lineCounter);
				    return false;
				}
			    }
			    else
			    {
				Logger::getInstance()->log(lError, "Invalid ip address format in line %d", lineCounter);
				return false;
			    }
			}
			else if(strcmp(keyword, "port") == 0)
			{
			    if(parsPort() == true)
			    {

			    }
			    else
			    {
				Logger::getInstance()->log(lError, "Invalid port value in line %d", lineCounter);
				return false;
			    }
			}
			else if(strcmp(keyword, "timeinterval") == 0)
			{
			    this->timeInterval = strtol(value, &pEnd, 10);
			    this->timeInterval *= 1000;
			}
			else if(strcmp(keyword, "daystosave") == 0)
			{
			    this->daysToSave = strtol(value, &pEnd, 10);
			}
			else if(strcmp(keyword, "maxfilesize") == 0)
			{
			    this->maxFileSize = strtol(value, &pEnd, 10);
			    this->maxFileSize *= 1000;
			}
			else if(strcmp(keyword, "maxtotalsize") == 0)
			{
			    this->maxTotalSize = strtol(value, &pEnd, 10);
			    this->maxTotalSize *= 1000;
			}
			else if(strcmp(keyword, "packetinterval") == 0)
			{
			    this->packetInterval = strtol(value, &pEnd, 10);
			}
			else if(strcmp(keyword, "path") == 0)
			{
// 			    if(checkDir() == true)
// 			    {
				this->path = value;
// 			    }
// 			    else
// 			    {
// 				Logger::getInstance()->log(lError, "Invalid value of keyword 'path' in line %d", lineCounter);
// 				return false;
// 			    }
			}
			else if(strcmp(keyword, "mountSrc") == 0)
			{
			    if(checkDir() == true)
			    {
				this->mountSrc = value;
			    }
			    else
			    {
				Logger::getInstance()->log(lError, "Invalid value of keyword 'mountSrc' in line %d", lineCounter);
				return false;
			    }
			}
			else if(strcmp(keyword, "mountDst") == 0)
			{
// 			    if(checkDir() == true)
// 			    {
				this->mountDst = value;
// 			    }
// 			    else
// 			    {
// 				Logger::getInstance()->log(lError, "Invalid value of keyword 'mountDst' in line %d", lineCounter);
// 				return false;
// 			    }
			}
			else if(strcmp(keyword, "debug") == 0)
			{
			    if(strcmp(value, "true") == 0)
			    {
				this->debug = true;
			    }
			    else if(strcmp(value, "false") == 0)
			    {
				this->debug = false;
			    }
			    else
			    {
				Logger::getInstance()->log(lError, "Incorrect value of keyword 'debug' in line %d", lineCounter);
				return false;
			    }
			}
			else
			{
			    Logger::getInstance()->log(lError, "Not recognized keyword '%s', in line %d", keyword, lineCounter);
			    return false;
			}
		      //}

		}
		else
		{
		    Logger::getInstance()->log(lError, "Not recognized line '%s', in line %d", line, lineCounter);
		    return false;
		}
	    }
	    else
	    {
		Logger::getInstance()->log(lError, "Error in line %d, '%s'", lineCounter, line);
		return false;
	    }
	}
        }
	else
	{
	    Logger::getInstance()->log(lWarning, "Empty line %d", lineCounter);
	}
    }

    //Remove resources
    if(keyword == NULL)
    {
        delete keyword;
        keyword = NULL;
    }

    if(value == NULL)
    {
        delete value;
        value = NULL;
    }

    printConfiguration();

    return true;
}

std::string Parser::getIp(void) const
{
    return ip;
}

int Parser::getTimeInterval(void) const
{
    return timeInterval;
}

int Parser::getMaxFileSize(void) const
{
    return maxFileSize;
}

int Parser::getDaysToSave(void) const
{
    return daysToSave;
}

uint64_t Parser::getMaxTotalSize(void) const
{
    return maxTotalSize;
}

bool Parser::getDebug(void) const
{
    return debug;
}

std::string Parser::getFilePath(void) const
{
    return path;
}

int Parser::getPacketInterval(void) const
{
    return packetInterval;
}

std::string Parser::getMountSrc(void) const
{
    return mountSrc;
}

std::string Parser::getMountDst(void) const
{
    return mountDst;
}

bool Parser::parsLine(void)
{
    uint8_t index = 0;

    for(uint8_t i = 0; i < strlen(line); i++)
    {
        if(line[i] > 32 && line[i] < 127)
        {
            line[index] = line[i];
            index++;
        }
    }
    line[index] = '\0';

    if(line[index - 1] == ';')
    {
        char* equalPointer = NULL;
        equalPointer = strchr(line, '=');

        if(equalPointer != NULL)
        {
            if(keyword != NULL)
            {
                delete keyword;
                keyword = NULL;
            }
            if(value != NULL)
            {
                delete value;
                value = NULL;
            }
            keyword = new char[equalPointer - line + 1];
            value = new char[&line[index - 1] - (equalPointer + 1) + 1];

            memcpy(keyword, line, equalPointer - line);
            keyword[equalPointer - line] = '\0';

            memcpy(value, equalPointer + 1, &line[index - 1] - (equalPointer + 1));
            value[&line[index - 1] - (equalPointer + 1)] = '\0';
            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}

bool Parser::parsIp(void)
{
    long int octet[4] = {-1, -1, -1, -1};
    int i = 0;
    int j = 0;

    octet[j++] = atoi(value);

    while(value[i] != '\0')
    {
        if(value[i] == '.')
        {
            octet[j++] = atoi(&value[i + 1]);
        }
        i++;
    }
    for(i = 0; i < 4; i++)
    {
        if(octet[i] < 0 || octet[i] > 255)
        {
            return false;
        }
    }
    this->ip = value;
    return true;
}

bool Parser::checkIp(void)
{
    int numberOfChars = 0;
    int numberOfOctets = 0;

    for(uint8_t i = 0; i < strlen(value); i++)
    {
        if(value[i] > 0x2F && value[i] < 0x3A)
        {
            if(numberOfChars < 3)
            {
                numberOfChars++;
            }
            else
            {
                return false;
            }
        }
        else if (value[i] == '.' && numberOfOctets < 4)
        {
            numberOfChars = 0;
            numberOfOctets++;
        }
        else
        {
            return false;
        }
    }
    return true;
}

bool Parser::parsPort(void)
{
    int i = 0;
    long int port = 0;

    while(value[i] != '\0')
    {
        if(value[i] < 0x30 || value[i] > 0x3A)
        {
            return false;
        }
        i++;
    };

    port = atoi(value);

    if(port > 0 && port <= 65535)
    {
        portList->push_back(port);
        return true;
    }
    else
    {
        return false;
    }
}

bool Parser::checkDir(void)
{
    struct stat s;

    if(stat(value, &s) == 0)
    {
        if(s.st_mode & S_IFDIR)
        {
            return true;
        }
        else
        {
            Logger::getInstance()->log(lError, "Invalid file path in line %d", lineCounter);
            return false;
        }
    }
    else
    {
        Logger::getInstance()->log(lError, "Invalid file path in line %d", lineCounter);
        return false;
    }
}

void Parser::printConfiguration(void)
{
    std::cout << "IP address:          [ " << BOLD GREEN  <<  this->ip.c_str() <<  RESET " ]\n";
    for(uint16_t i = 0; i < this->portList->size(); i++)
    {
        std::cout << "Port[" << i + 1 << "]:             [ " << BOLD GREEN << portList->at(i) << RESET " ]\n";
    }
    std::cout << "Time interval [s]:   [ " << BOLD GREEN << (timeInterval / 1000) << RESET " ]\n";
    std::cout << "Log path:            [ " << BOLD GREEN << path.c_str() << RESET << " ]\n";
    std::cout << "Packet interval:     [ " << BOLD GREEN << packetInterval << RESET " ]\n";
    std::cout << "Days to save:        [ " << BOLD GREEN << daysToSave << RESET " ]\n";
    std::cout << "Max file size [kb]:  [ " << BOLD GREEN << (maxFileSize / 1024) << RESET " ]\n";
    std::cout << "Max total size [kb]: [ " << BOLD GREEN << (maxTotalSize / 1024) << RESET " ]\n";
    std::cout << "Mount dst:           [ " << BOLD GREEN << mountDst.c_str() << RESET << " ]\n";
    std::cout << "Mount src:           [ " << BOLD GREEN << mountSrc.c_str() << RESET << " ]\n";
    std::cout << "Debug              : [ " << BOLD GREEN << (debug == true ? "true" : "false") << RESET " ]\n";
}


int Parser::getPortList(void) const
{
    return portList->size();
}


int Parser::getPort(uint16_t port) const
{
    if(port < portList->size())
    {
	return portList->at(port);
    }
    else
    {
      return -1;
    }
}
