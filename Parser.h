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
	virtual bool read();
	virtual std::string getIp(void) const;
	virtual int getTimeInterval(void) const;
	virtual int getMaxFileSize(void) const;
	virtual int getDaysToSave(void) const;
	virtual uint64_t getMaxTotalSize(void) const;
	virtual bool getDebug(void) const;
	virtual std::string getFilePath(int index) const;
	virtual int getPacketInterval(void) const;
	virtual std::string getMountSrc(void) const;
	virtual std::string getMountDst(void) const;
	virtual int getPortList(void) const;
	virtual int getPort(uint16_t port) const;

protected:
	virtual bool parsLine(void);
	virtual bool parsIp(void);
	virtual bool checkIp(void);
	virtual bool parsPort(void);
	virtual bool checkDir(void);
	virtual void printConfiguration(void);

private:
	std::string mountSrc;
	std::string mountDst;

	char *line;
	char *keyword;
	char *value;
	bool parsError;
	uint8_t lineCounter;
};

#endif /* PARSER_H_ */
