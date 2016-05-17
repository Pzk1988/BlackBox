/*
 * FileStorageTests.cpp
 *
 *  Created on: May 17, 2016
 *      Author: pk
 */
#include <gmock/gmock.h>
#include <stdint.h>
#include <string>
#include <gtest/gtest.h>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include "Mock/SystemCommandsMock.h"
#include "Mock/ParserMock.h"
#include "../Buffer.h"
#include "../FileStorage.h"

using ::testing::AtLeast;

TEST(FileStorageTests, noMount)
{
	ParserMock *parserMock = new ParserMock("");
	SystemCommandsMock *systemCommandsMock = new SystemCommandsMock();
	FileStorage *fileStorage = new FileStorage(parserMock, systemCommandsMock, 0);

	EXPECT_CALL(*parserMock, getPacketInterval())
				.Times(1)
				.WillOnce(Return(1));

	EXPECT_CALL(*parserMock, getMountSrc())
					.Times(1)
					.WillOnce(Return(""));

	EXPECT_CALL(*parserMock, getMountDst())
					.Times(0);

	EXPECT_CALL(*parserMock, getFilePath(0))
					.Times(AtLeast(1))
					.WillOnce(Return(""))
					.WillRepeatedly(Return(""));

	EXPECT_CALL(*systemCommandsMock, createDir(""))
					.Times(AtLeast(1))
					.WillOnce(Return(0));

	EXPECT_EQ(fileStorage->init(), -1);

	delete parserMock;
	delete systemCommandsMock;
}

/*TEST(FileStorageTests, mountFail)
{
	ParserMock *parserMock = new ParserMock("");
	SystemCommandsMock *systemCommandsMock = new SystemCommandsMock();
	FileStorage *fileStorage = new FileStorage(parserMock, systemCommandsMock);

	EXPECT_CALL(*parserMock, getPacketInterval())
				.Times(1)
				.WillOnce(Return(1));

	EXPECT_CALL(*parserMock, getMountSrc())
					.Times(1)
					.WillOnce(Return(" "));

	EXPECT_CALL(*parserMock, getMountDst())
					.Times(0);

	EXPECT_CALL(*parserMock, getFilePath())
					.Times(AtLeast(1))
					.WillOnce(Return(""))
					.WillRepeatedly(Return(""));

	EXPECT_CALL(*systemCommandsMock, createDir(""))
					.Times(AtLeast(1))
					.WillOnce(Return(0));

	EXPECT_EQ(fileStorage->init(), -1);

	delete parserMock;
	delete systemCommandsMock;
}*/

TEST(FileStorageTests, impossibleToCreateDstDir)
{
	ParserMock *parserMock = new ParserMock("");
	SystemCommandsMock *systemCommandsMock = new SystemCommandsMock();
	FileStorage *fileStorage = new FileStorage(parserMock, systemCommandsMock, 0);

	EXPECT_CALL(*parserMock, getPacketInterval())
				.Times(1)
				.WillOnce(Return(1));

	EXPECT_CALL(*parserMock, getMountSrc())
					.Times(1)
					.WillOnce(Return(""));

	EXPECT_CALL(*parserMock, getMountDst())
					.Times(0);

	EXPECT_CALL(*parserMock, getFilePath(0))
					.Times(AtLeast(1))
					.WillOnce(Return(""))
					.WillRepeatedly(Return(""));

	EXPECT_CALL(*systemCommandsMock, createDir(""))
					.Times(AtLeast(1))
					.WillOnce(Return(-1));

	EXPECT_EQ(fileStorage->init(), -1);

	delete parserMock;
	delete systemCommandsMock;
}

