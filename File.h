/*
 * File.h
 *
 *  Created on: Apr 10, 2016
 *      Author: pk
 */

#ifndef FILE_H_
#define FILE_H_

#include <string>
#include <stdint.h>

class File
{
public:
	File(std::string name, uint64_t size);
	virtual ~File();
	void setName(std::string name){this->name = name;}
	void setSize(int size){this->size = size;}
	std::string getName(void) const {return this->name;}
	int getSize(void) const {return this->size;}
private:
	std::string name;
	int size;
};

#endif /* FILE_H_ */
