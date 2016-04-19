/*
 * Buffer.h
 *
 *  Created on: Apr 19, 2016
 *      Author: pk
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <inttypes.h>

#define BUFFER_SIZE 20
class Buffer {
public:
	Buffer();
	virtual ~Buffer();
	bool isFull(void);
	bool isEmpty(void);
	bool add(uint8_t const * const pData, int size);
	bool get(uint8_t * const pData, int *size, int i);
	uint64_t delayTime(void);
private:
	uint8_t **pBuff;
	uint8_t *pBuffSize;
	uint8_t bufferPointer;
	uint64_t storeTime;
};

#endif /* BUFFER_H_ */
