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

Buffer::Buffer(uint32_t _packageInterval)
{
	packageInterval = _packageInterval;
	bufferPointer = 0;
	storeTime = 0;
	pBuff = new uint8_t*[packageInterval];
	pBuffSize = new uint8_t[packageInterval];

	for(uint32_t i = 0; i < packageInterval; i++)
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
		return true;
	}
	else
	{
		return false;
	}
}


bool Buffer::isFull(void)
{
	if(bufferPointer == packageInterval)
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
