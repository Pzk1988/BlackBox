/*
 * Buffer.cpp
 *
 *  Created on: Apr 19, 2016
 *      Author: pk
 */

#include "Buffer.h"
#include <string.h>
#include "TimeCounter.h"
#include <iostream>
#include <stdio.h>

Buffer::Buffer(uint32_t _packageInterval)
{
	this->packageInterval = _packageInterval;
	this->bufferSize = 0;
	this->storeTime = 0;
	msgBuffer = new MsgBuffer[packageInterval];
	this->ptrFirst = &msgBuffer[0];
	this->ptrLast = &msgBuffer[0];
}

Buffer::~Buffer() {
	// TODO Auto-generated destructor stub
}

bool Buffer::add(uint8_t const * const pData, int size)
{
	if(isEmpty() == true)
	{
		storeTime = TimeCounter::getInstance()->getTime();
	}

	if(isFull() != true)
	{
		ptrLast->size = size + 1;
		memcpy(ptrLast->data, pData, size);
		ptrLast->data[size] = '\n';
		this->bufferSize++;
		
		if(ptrLast == &msgBuffer[packageInterval - 1])
		{
		  ptrLast = &msgBuffer[0];
		}
		else
		{
		  ptrLast++;
		}
		return true;
	}
	else
	{
		return false;
	}
}

bool Buffer::get(uint8_t * const pData, int *size)
{
	if(isEmpty() != true)
	{
		*size = ptrFirst->size;
		memcpy(pData, &ptrFirst->data, *size);
		this->bufferSize--;
		
		if(ptrFirst == &msgBuffer[packageInterval - 1])
		{
		  ptrFirst = &msgBuffer[0];
		}
		else
		{
		  ptrFirst++;
		}
		
		return true;
	}
	else
	{
		return false;
	}
}


bool Buffer::isFull(void)
{
	if(this->bufferSize == packageInterval)
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
	if(this->bufferSize == 0)
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
