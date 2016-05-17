/*
 * iParser.h
 *
 *  Created on: Apr 4, 2016
 *      Author: pk
 */

#ifndef IPARSER_H_
#define IPARSER_H_
#include <string>
#include <stdint.h>
#include <vector>

class IParser
{
public:
	virtual ~IParser(){};
	virtual bool read() = 0;
	virtual std::string getIp(void) const = 0;
	virtual int getTimeInterval(void) const = 0;
	virtual int getMaxFileSize(void) const = 0;
	virtual int getDaysToSave(void) const = 0;
	virtual uint64_t getMaxTotalSize(void) const = 0;
	virtual bool getDebug(void) const = 0;
	virtual std::string getFilePath(int index) const = 0;
	virtual int getPacketInterval(void) const = 0;
	virtual int getPortList(void) const = 0;
protected:
	std::string diagnPath;
	std::string ip;
	int timeInterval;
	int maxFileSize;
	int daysToSave;
	uint64_t maxTotalSize;
	bool debug;
	int packetInterval;

	std::vector<uint16_t> *portList;
	std::vector<std::string> *storePath;
};

#endif /* IPARSER_H_ */
