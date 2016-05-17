/*
 * Buffer.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pk
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <inttypes.h>
#include "global.h"

struct MsgBuffer
{
  uint8_t data[MAX_MSG_LEN];
  uint16_t size;
};

class Buffer {
public:
	Buffer(uint32_t _packageInterval);
	virtual ~Buffer();
	bool isFull(void);
	bool isEmpty(void);
	bool add(uint8_t const * const pData, int size);
	bool get(uint8_t * const pData, int *size);
	uint64_t delayTime(void);
private:
	MsgBuffer *msgBuffer;
	uint8_t bufferSize;
	MsgBuffer *ptrFirst;
	MsgBuffer *ptrLast;
	uint64_t storeTime;
	uint32_t packageInterval;
};

#endif /* BUFFER_H_ */
