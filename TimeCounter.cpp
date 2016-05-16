/*
 * TimeCounter.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: pk
 */

#include <stddef.h>
#include <stdio.h>
#include "global.h"
#include "TimeCounter.h"

TimeCounter* TimeCounter::instance = NULL;
uint64_t TimeCounter::startValue = 0;
uint64_t TimeCounter::stopValue = 0;
struct timespec TimeCounter::ts = {0, 0};

TimeCounter::TimeCounter() {
	// TODO Auto-generated constructor stub

}

TimeCounter::~TimeCounter() {
	// TODO Auto-generated destructor stub
}

TimeCounter* TimeCounter::getInstance(void)
{
	if(instance == NULL)
	{
		instance = new TimeCounter();
		return instance;
	}
	else
	{
		return instance;
	}
}

void TimeCounter::start(void)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	startValue = ts.tv_nsec + (ts.tv_sec * 1000 * 1000);
}

uint64_t TimeCounter::stop(void)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	stopValue = ts.tv_nsec + (ts.tv_sec * 1000 * 1000);

	return stopValue - startValue;
}

uint64_t TimeCounter::getTime(void)
{
	clock_gettime(CLOCK_MONOTONIC_RAW, &ts);
	return  (ts.tv_nsec / 1000000) + (ts.tv_sec * 1000);
}
