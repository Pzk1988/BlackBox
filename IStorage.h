/*
 * iStorage.h
 *
 *  Created on: Apr 8, 2016
 *      Author: pk
 */

#ifndef ISTORAGE_H_
#define ISTORAGE_H_
#include <string>

class IStorage
{
public:
	virtual void operator<< (std::string)=0;
	virtual int init(void)=0;
	virtual ~IStorage(){};
};

#endif /* ISTORAGE_H_ */
