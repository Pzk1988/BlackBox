#include <iostream>
#include <gtest/gtest.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "Parser.h"
#include "global.h"
#include "Logger.h"
#include "FileStorage.h"
#include "UdpReceiver.h"

//#define RUN_TESTS

//Globals
FileStorage *storage;

struct BlackBoxHandler
{
	UdpReceiver *udpReceiver;
	FileStorage *fileStorage;
};

void *receiveThreadRoutine(void *arg);

const std::string version = "0.0.0.1";

void printInfo(void)
{
    std::cout << "***************************************\n";
    std::cout << "*                                     *\n";
    std::cout << "*         " << BOLD << YELLOW << " EBI Lock 400 BBox " << RESET << "         *\n";
    std::cout << "*            " BOLD YELLOW " UDP Logger " RESET "             *\n";
    std::cout << "*            " BOLD YELLOW "  v 2.0.1 " RESET "               *\n";
    std::cout << "*                                     *\n";
    std::cout << "***************************************\n";
    std::cout << "\n\n";
}

void signalHandler(int signalID)
{
	std::cout << "Received interrupt signal: " + signalID;
	delete storage;
	exit(0);
}

void registerSignals(void)
{
	signal(SIGINT, signalHandler);
	signal(SIGKILL, signalHandler);
	signal(SIGTERM, signalHandler);
}

int main(int argc, char **argv)
{
#ifdef RUN_TESTS
	Logger::getInstance()->log(lWarning, "BlackBox version %s tests", version.c_str());
	//Run unit tests
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
#else

	BlackBoxHandler blackBoxHandler;
	pthread_t recvThread;

	//Print version
	printInfo();

	//Register linux interrupt signal handlers
	registerSignals();

	//Create parser instance
	Parser *parser = new Parser(argv[1]);

	//Read configuration file
	if(parser->read() == true)
	{
		//Create system command executor
		SystemCommands *cmd = new SystemCommands(parser->getMountDst());

		//Create file handler
		storage = new FileStorage(parser, cmd);

		//Create udp socket
		UdpReceiver *udpReceiver = new UdpReceiver();

		if(storage->init() == 0)
		{
			if(udpReceiver->init(3000) == 0)
			{
				//Prepare argument for thread
				blackBoxHandler.fileStorage = storage;
				blackBoxHandler.udpReceiver = udpReceiver;

				//Start receive thread
				pthread_create(&recvThread, NULL, &receiveThreadRoutine, &blackBoxHandler);

				while(1)
				{
					//Check if new file needs to be created and if buffer needs to be flushed
					storage->update();
					sleep(5);
				}
			}
			else
			{
				return -1;
			}
		}
		else
		{
			return -1;
		}
	}
	else
	{
		return -1;
	}

#endif
}

void* receiveThreadRoutine(void *rec)
{
	uint8_t buff[MAX_MSG_LEN];
	uint16_t size = 0;
	UdpReceiver *udpReceiver = ((BlackBoxHandler*)rec)->udpReceiver;
	FileStorage *fileStorage = ((BlackBoxHandler*)rec)->fileStorage;

	while(1)
	{
		size = udpReceiver->receiver(buff);
		fileStorage->add(buff, size);
	}
	return NULL;
}

