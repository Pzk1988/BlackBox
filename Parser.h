/*
 * Parser.h
 *
 *  Created on: Apr 4, 2016
 *      Author: Przemysław Knapik
 */

#ifndef PARSER_H_
#define PARSER_H_

#include <string>

#include "IParser.h"

class Parser : public IParser
{
public:
	Parser(std::string path);
	virtual ~Parser();
	bool read();
	std::string getIp(void) const;
	int getTimeInterval(void) const;
	int getMaxFileSize(void) const;
	int getDaysToSave(void) const;
	uint64_t getMaxTotalSize(void) const;
	bool getDebug(void) const;
	std::string getFilePath(void) const;
	int getPacketInterval(void) const;
	std::string getMountSrc(void) const;
	std::string getMountDst(void) const;
	int getPortList(void) const;

private:
	std::string mountSrc;
	std::string mountDst;

	char* line = NULL;
	char keyword[20];
	char value[20];
	bool parsError;
	uint8_t lineCounter;

	bool parsLine(void);
	bool parsIp(void);
	bool checkIp(void);
	bool parsPort(void);
	bool checkDir(void);
	void printConfiguration(void);
};

#endif /* PARSER_H_ */
