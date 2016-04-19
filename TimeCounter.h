/*
 * TimeCounter.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pk
 */

#ifndef TIMECOUNTER_H_
#define TIMECOUNTER_H_

#include <stdint.h>
#include <unistd.h>
#include <time.h>

class TimeCounter
{
public:
	static TimeCounter* getInstance(void);
	void start(void);
	uint64_t stop(void);
	uint64_t getTime(void);
private:
	TimeCounter();
	static TimeCounter *instance;
	virtual ~TimeCounter();
	static uint64_t startValue;
	static uint64_t stopValue;
	static struct timespec ts;
};

#endif /* TIMECOUNTER_H_ */
