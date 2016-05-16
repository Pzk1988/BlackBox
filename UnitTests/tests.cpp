/*
 * tests.cpp
 *
 *  Created on: Apr 5, 2016
 *      Author: pk
 */

#include <gmock/gmock.h>
#include <string>
#include <gtest/gtest.h>
#include <exception>
#include <stdexcept>
#include <typeinfo>
#include "SystemCommandsMock.h"
#include "../FileHandler.h"

using ::testing::AtLeast;
TEST(Addition, CanAddTwoNumbers)
{
  SystemCommandsMock systemCommandsMock("", "/home/zwspkik");
  
  FileHandler fileHandler(&systemCommandsMock);
  
  EXPECT_CALL(systemCommandsMock, umount()).Times(1);

  fileHandler.open("/home/cos/cos.txt");

}

TEST(Addition, CanAddTwoNumbers1)
{
  SystemCommandsMock systemCommandsMock("", "/home/zwspkik");

  FileHandler fileHandler(&systemCommandsMock);

  EXPECT_CALL(systemCommandsMock, umount()).Times(0);

  fileHandler.open("test.txt");

}


