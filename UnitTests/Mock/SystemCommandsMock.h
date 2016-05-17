#ifndef SYSTEMCOMMANDSMOCK_H
#define SYSTEMCOMMANDSMOCK_H
#include "../../SystemCommands.h"
#include <gmock/gmock.h>
#include <string>
#include <gtest/gtest.h>
#include <exception>
#include <stdexcept>
#include <typeinfo>

using ::testing::Return;

class SystemCommandsMock : public SystemCommands
{
public:
	SystemCommandsMock() : SystemCommands("", ""){}
	MOCK_CONST_METHOD1(remove, int(std::string file));
	MOCK_CONST_METHOD1(create, int(std::string file));
	MOCK_CONST_METHOD2(mount, int(std::string src, std::string dst));
	MOCK_CONST_METHOD0(umount, void());
	MOCK_CONST_METHOD1(createDir, int(std::string path));
};

#endif 
