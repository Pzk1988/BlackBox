/*
 * Buffer.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: pk
 */

#include "Buffer.h"
#include "global.h"
#include <string.h>
#include "TimeCounter.h"
#include <iostream>

Buffer::Buffer()
{
	bufferPointer = 0;
	storeTime = 0;
	pBuff = new uint8_t*[BUFFER_SIZE];
	pBuffSize = new uint8_t[BUFFER_SIZE];

	for(int i = 0; i < BUFFER_SIZE; i++)
	{
		pBuff[i] = new uint8_t[MAX_MSG_LEN];
	}
}

Buffer::~Buffer() {
	// TODO Auto-generated destructor stub
}

bool Buffer::add(uint8_t const * const pData, int size)
{
	if(isEmpty() == true)
	{
		storeTime = TimeCounter::getInstance()->getTime();
		//std::cout << "Timer od nowa " << storeTime << std::endl;
	}

	if(isFull() != true)
	{
		std::cout << "Buffer add " << (int)bufferPointer << std::endl;
		pBuffSize[bufferPointer] = size + 1;
		memcpy(&pBuff[bufferPointer][0], pData, size);
		pBuff[bufferPointer][size] = '\n';
		pBuff[bufferPointer++][size + 1] = '\0';
		return true;
	}
	else
	{
		return false;
	}
}

bool Buffer::get(uint8_t * const pData, int *size, int i)
{
	if(isEmpty() != true)
	{
		bufferPointer--;
		*size = pBuffSize[i];
		memcpy(pData, &pBuff[bufferPointer][0], *size);
		std::cout << "Buffer remove " << (int)bufferPointer << std::endl;
		return true;
	}
	else
	{
		return false;
	}
}


bool Buffer::isFull(void)
{
	if(bufferPointer == BUFFER_SIZE)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Buffer::isEmpty(void)
{
	if(bufferPointer == 0)
	{
		return true;
	}
	else
	{
		return false;
	}
}

uint64_t Buffer::delayTime(void)
{
	if(isEmpty() != true)
	{
		return TimeCounter::getInstance()->getTime() - storeTime;
	}
	else
	{
		return 0;
	}

}
