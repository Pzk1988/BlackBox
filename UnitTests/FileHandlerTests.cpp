/*
 * tests.cpp
 *
 *  Created on: Apr 5, 2016
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
#include "../FileHandler.h"

using ::testing::AtLeast;
TEST(FileHandler, inCorrectPath)
{
  SystemCommandsMock systemCommandsMock;
  
  FileHandler *fileHandler = new FileHandler(&systemCommandsMock);
  
  EXPECT_CALL(systemCommandsMock, umount()).Times(1);

  uint8_t tab[10];
  fileHandler->open("/home/cos/cos.txt");
  fileHandler->write(tab, 10);
  fileHandler->flush();
  fileHandler->close();
  delete fileHandler;
}

TEST(FileHandler, correctPath)
{
  SystemCommandsMock systemCommandsMock;

  FileHandler *fileHandler = new FileHandler(&systemCommandsMock);

  EXPECT_CALL(systemCommandsMock, umount()).Times(0);

  uint8_t tab[10];
  fileHandler->open("test.txt");
  fileHandler->write(tab, 10);
  fileHandler->flush();
  delete fileHandler;
}

TEST(FileHandler, writeIncorrectPath)
{
  SystemCommandsMock systemCommandsMock;

  FileHandler *fileHandler = new FileHandler(&systemCommandsMock);

  EXPECT_CALL(systemCommandsMock, umount()).Times(0);

  uint8_t tab[10];

  fileHandler->write(tab, 10);
  delete fileHandler;
}


