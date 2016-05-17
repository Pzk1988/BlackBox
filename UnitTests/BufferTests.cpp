/*
 * BufferTests.cpp
 *
 *  Created on: May 16, 2016
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
#include "../Buffer.h"

using ::testing::AtLeast;

TEST(BufferTests, addRemoveTest)
{
	int size;
	uint8_t tab[30];

	Buffer *buffer = new Buffer(20);
	EXPECT_EQ(true, buffer->isEmpty());
	EXPECT_EQ(false, buffer->isFull());

	EXPECT_EQ(true, buffer->add(tab, 1));

	EXPECT_EQ(false, buffer->isEmpty());
	EXPECT_EQ(false, buffer->isFull());

	for(int i = 0; i < 18; i++)
	{
		EXPECT_EQ(true, buffer->add(tab, i + 2));
	}

	EXPECT_EQ(false, buffer->isEmpty());
	EXPECT_EQ(false, buffer->isFull());

	EXPECT_EQ(true, buffer->add(tab, 20));

	EXPECT_EQ(false, buffer->isEmpty());
	EXPECT_EQ(true, buffer->isFull());

	EXPECT_EQ(true, buffer->get(tab, &size));

	EXPECT_EQ(false, buffer->isEmpty());
	EXPECT_EQ(false, buffer->isFull());
	EXPECT_EQ(1 + 1, size);

	EXPECT_EQ(true, buffer->get(tab, &size));

	EXPECT_EQ(false, buffer->isEmpty());
	EXPECT_EQ(false, buffer->isFull());
	EXPECT_EQ(2 + 1, size);

	delete buffer;
}

TEST(BufferTests, addToFullBuffer)
{
	int size;
	uint8_t tab[3001];

	Buffer *buffer = new Buffer(20);

	EXPECT_EQ(true, buffer->isEmpty());
	EXPECT_EQ(false, buffer->isFull());

	for(int i = 0; i < 20; i++)
	{
		EXPECT_EQ(true, buffer->add(tab, 3000));
	}

	EXPECT_EQ(true, buffer->isFull());
	EXPECT_EQ(false, buffer->add(tab, 3000));
	EXPECT_EQ(false, buffer->add(tab, 3000));
	EXPECT_EQ(true, buffer->get(tab, &size));
	EXPECT_EQ(size, 3001);
	EXPECT_EQ(true, buffer->add(tab, 3000));
}

TEST(BufferTests, removeFromEmptyBuffer)
{
	int size;
	uint8_t tab[3001];

	Buffer *buffer = new Buffer(20);

	EXPECT_EQ(false, buffer->get(tab, &size));

	for(int i = 0; i < 20; i++)
	{
		EXPECT_EQ(true, buffer->add(tab, i));
	}

	for(int i = 0; i < 20; i++)
	{
		EXPECT_EQ(true, buffer->get(tab, &size));
		EXPECT_EQ(size, i + 1);
	}
}

TEST(BufferTests, storeTime)
{
	int size;
	uint8_t tab[3001];

	Buffer *buffer = new Buffer(20);

	EXPECT_EQ(0, buffer->delayTime());

	EXPECT_EQ(true, buffer->add(tab, 10));

	usleep(1000);

	EXPECT_GT(buffer->delayTime(), 0);
}

TEST(BufferTests, longTermUse)
{
	int size;
	uint8_t tab[3001];

	Buffer *buffer = new Buffer(20);

	for(int i = 0; i < 15; i++)
	{
		memset(tab, 0x30 + i, 3000);
		EXPECT_EQ(true, buffer->add(tab, 3000));
	}

	for(int i = 0; i < 5; i++)
	{
		EXPECT_EQ(true, buffer->get(tab, &size));
		EXPECT_EQ(3001, size);
		for(int j = 0; j < size - 1; j++)
		{
			EXPECT_EQ(tab[j], 0x30 + i);
		}
		EXPECT_EQ(tab[size - 1], 0x0A);
	}

	for(int i = 0; i < 5; i++)
	{
		memset(tab, 0xA0 + i, 3000);
		EXPECT_EQ(true, buffer->add(tab, 3000));
	}

	for(int i = 0; i < 5; i++)
	{
		EXPECT_EQ(true, buffer->get(tab, &size));
		EXPECT_EQ(3001, size);
		for(int j = 0; j < size - 1; j++)
		{
			EXPECT_EQ(tab[j], 0x30 + i + 5);
		}
		EXPECT_EQ(tab[size - 1], 0x0A);
	}

	for(int i = 0; i < 9; i++)
	{
		memset(tab, 0xC0 + i, 3000);
		EXPECT_EQ(true, buffer->add(tab, 3000));
	}


	for(int i = 0; i < 5; i++)
	{
		EXPECT_EQ(true, buffer->get(tab, &size));
		EXPECT_EQ(3001, size);
		for(int j = 0; j < size - 1; j++)
		{
			EXPECT_EQ(tab[j], 0x30 + i + 10);
		}
		EXPECT_EQ(tab[size - 1], 0x0A);
	}

	for(int i = 0; i < 5; i++)
	{
		EXPECT_EQ(true, buffer->get(tab, &size));
		EXPECT_EQ(3001, size);
		for(int j = 0; j < size - 1; j++)
		{
			EXPECT_EQ(tab[j], 0xA0 + i);
		}
		EXPECT_EQ(tab[size - 1], 0x0A);
	}

	for(int i = 0; i < 9; i++)
	{
		EXPECT_EQ(true, buffer->get(tab, &size));
		EXPECT_EQ(3001, size);
		for(int j = 0; j < size - 1; j++)
		{
			EXPECT_EQ(tab[j], 0xC0 + i);
		}
		EXPECT_EQ(tab[size - 1], 0x0A);
	}

	EXPECT_EQ(true, buffer->isEmpty());
}
