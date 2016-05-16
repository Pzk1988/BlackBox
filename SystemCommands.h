/*
 * SystemCommands.h
 *
 *  Created on: Apr 13, 2016
 *      Author: pk
 */

#ifndef SYSTEMCOMMANDS_H_
#define SYSTEMCOMMANDS_H_
#include <string>
#include <gtest/gtest.h>

class SystemCommands {
public:
	SystemCommands(std::string _path, std::string _mountPath);
	virtual ~SystemCommands();
	virtual int remove(std::string file) const;
	virtual int create(std::string file) const;
	virtual int mount(std::string src, std::string dst) const;
	virtual void umount() const;
	virtual bool createDir(std::string path) const;
protected:
	const std::string path;
	const std::string mountPath;
};

#endif /* SYSTEMCOMMANDS_H_ */
