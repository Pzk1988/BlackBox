/*
 * File.cpp
 *
 *  Created on: Apr 10, 2016
 *      Author: pk
 */

#include "File.h"

File::File(std::string name, uint64_t size)
{
	this->name = name;
	this->size = size;
}

File::~File() {
	// TODO Auto-generated destructor stub
}

