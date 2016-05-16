/*
 * SystemCommands.cpp
 *
 *  Created on: Apr 13, 2016
 *      Author: pk
 */

#include "SystemCommands.h"
#include "Logger.h"
#include <stdlib.h>
#include <iostream>
#include <sys/stat.h>

SystemCommands::SystemCommands(std::string _path, std::string _mountPath) : path(_path + '/'), mountPath(_mountPath)
{

}

SystemCommands::~SystemCommands()
{
	// TODO Auto-generated destructor stub
}

int SystemCommands::remove(std::string file) const
{
	int result;
	//struct stat st;

#warning solve different owner or lack of permissions
//	stat((path + file).c_str(), &st);
//	if((st.st_mode & S_IWUSR) != 0)
//	{
		std::string filePath = "rm " + path + file;
		result = system(filePath.c_str());
		
		if(result != 0)
		{
			Logger::getInstance()->log(lError, "Remove of file %s failed, exit code %08x", file.c_str(), result);
			return -1;
		}
		else
		{
			Logger::getInstance()->log(lInfo, "Removed file %s", file.c_str());
			return 0;
		}
//	}
//	else
//	{
//		Logger::getInstance()->log(lError, "Remove of file %s failed, user does not have permission to write", file.c_str());
//		return -1;
//	}
}

int SystemCommands::create(std::string file) const
{
	int result;
	std::string filePath = "touch " + path + file.c_str();
	result = system(filePath.c_str());
	
	if(result != 0)
	{
		Logger::getInstance()->log(lError, "Creating of file %s failed, exit code %08x", filePath.c_str(), result);
		umount();
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Created file %s", file.c_str());
	}
	return result;
}

int SystemCommands::mount(std::string src, std::string dst) const
{
	int result;
	std::string cmd;
	cmd = "mount " + src + " " + dst;
	result = system(cmd.c_str());

	if(result == 0)
	{
		Logger::getInstance()->log(lInfo, "Mounted %s to %s", src.c_str(), dst.c_str());
		return result;
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Failed to mount %s to %s, return code %08x", src.c_str(), dst.c_str(), result);
		umount();
		return result;
	}
}

bool SystemCommands::createDir(std::string path) const
{
	int result;
	std::string cmd;
	cmd = "mkdir -p " + path;
	result = system(cmd.c_str());

	if(result == 0)
	{
		Logger::getInstance()->log(lInfo, "Created dir %s", path.c_str());
		return result;
	}
	else
	{
		Logger::getInstance()->log(lInfo, "Failed to create dir, return code %08x", path.c_str(), result);
		umount();
		return result;
	}
}

void SystemCommands::umount() const
{
    if(mountPath.empty() == false)
    {
        int result;
        std::string cmd;
        cmd = "umount -l " + mountPath;
        result = system(cmd.c_str());

        if(result == 0)
        {
            Logger::getInstance()->log(lInfo, "Unmounted dir %s", mountPath.c_str());
        }
        exit(-1);
    }
}

