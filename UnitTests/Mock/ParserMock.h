/*
 * ParserMock.h
 *
 *  Created on: May 17, 2016
 *      Author: pk
 */

#ifndef PARSERMOCK_H_
#define PARSERMOCK_H_
#include <gmock/gmock.h>
#include <string>
#include <gtest/gtest.h>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include "../../Parser.h"

using ::testing::Return;

class ParserMock : public Parser
{
public:
	ParserMock(std::string path) : Parser(""){}
	MOCK_METHOD0(read, bool());
	MOCK_CONST_METHOD0(getIp, std::string());
	MOCK_CONST_METHOD0(getTimeInterval, int());
	MOCK_CONST_METHOD0(getMaxFileSize, int());
	MOCK_CONST_METHOD0(getDaysToSave, int());
	MOCK_CONST_METHOD0(getMaxTotalSize, uint64_t());
	MOCK_CONST_METHOD0(getDebug, bool());
	MOCK_CONST_METHOD1(getFilePath, std::string(int));
	MOCK_CONST_METHOD0(getPacketInterval, int());
	MOCK_CONST_METHOD0(getMountSrc, std::string());
	MOCK_CONST_METHOD0(getMountDst, std::string());
	MOCK_CONST_METHOD0(getPortList, int());
	MOCK_CONST_METHOD1(getPort, int(uint16_t));

//protected:
//	virtual bool parsLine(void);
//	virtual bool parsIp(void);
//	virtual bool checkIp(void);
//	virtual bool parsPort(void);
//	virtual bool checkDir(void);
//	virtual void printConfiguration(void);

};

#endif /* PARSERMOCK_H_ */
