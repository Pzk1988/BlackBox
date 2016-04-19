/*
 * SystemCommands.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pk
 */

#ifndef SYSTEMCOMMANDS_H_
#define SYSTEMCOMMANDS_H_
#include <string>

class SystemCommands {
public:
	SystemCommands(std::string _path);
	virtual ~SystemCommands();
	int remove(std::string file) const;
	int create(std::string file) const;
	int mount(std::string src, std::string dst) const;
private:
	const std::string path;
};

#endif /* SYSTEMCOMMANDS_H_ */
