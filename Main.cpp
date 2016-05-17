#include <iostream>
#include <gtest/gtest.h>
#include <gmock/gmock.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>
#include <unistd.h>

#include "Parser.h"
#include "global.h"
#include "Logger.h"
#include "FileStorage.h"
#include "UdpReceiver.h"

//Globals
FileStorage **storage;
Parser *parser;
static bool checkDiagnFile(int argc, char** argv);

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
    std::cout << "*            " BOLD YELLOW 	  	"  " << VERSION << " " 		RESET "               *\n";
    std::cout << "*                                     *\n";
    std::cout << "***************************************\n";
    std::cout << "\n\n";
}

void signalHandler(int signalID)
{
    std::cout << "Received interrupt signal: " << signalID << "\n";

	for(int i = 0; i < parser->getPortList(); i++)
	{
		delete storage[i];
	}

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
    Logger::getInstance()->log(lError, "BlackBox version %s tests", version.c_str());
    //Run unit tests
    ::testing::InitGoogleMock(&argc, argv);
    //::testing::GTEST_FLAG(filter) = "BufferTests*";
    return RUN_ALL_TESTS();
    
#else
    if(checkDiagnFile(argc, argv) == false)
    {
    	Logger::getInstance()->log(lError, "Incorrect config file");
        return -1;
    }

    BlackBoxHandler *blackBoxHandler;
    pthread_t *recvThread;
    SystemCommands *cmd;

    //Print version
    printInfo();

    //Register linux interrupt signal handlers
    registerSignals();

    //Create parser instance
    parser = new Parser(argv[1]);

    //Read configuration file
    if(parser->read() == true)
    {
    	blackBoxHandler = new BlackBoxHandler[parser->getPortList()];
    	recvThread = new pthread_t[parser->getPortList()];
    	storage = new FileStorage*[parser->getPortList()];

    	//Init connection
    	for(int i = 0; i < parser->getPortList(); i++)
    	{
			//Create system command executor
			cmd = new SystemCommands(parser->getFilePath(i), parser->getMountDst());

			//Create file handler
			storage[i] = new FileStorage(parser, cmd, i);

			//Create udp socket
			UdpReceiver *udpReceiver = new UdpReceiver();

			if(storage[i]->init() == 0)
			{
				if(udpReceiver->init(parser->getPort(i)) == 0)
				{
					//Prepare argument for thread
					blackBoxHandler[i].fileStorage = storage[i];
					blackBoxHandler[i].udpReceiver = udpReceiver;

					//Start receive thread
					pthread_create(&recvThread[i], NULL, &receiveThreadRoutine, &blackBoxHandler[i]);
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

    	//Runtime
		while(1)
		{
			for(int i = 0; i < parser->getPortList(); i++)
			{
				//Check if new file needs to be created and if buffer needs to be flushed
				storage[i]->update();
			}
			sleep(1);
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

bool checkDiagnFile(int argc, char** argv)
{
    if(argc == 2)
    {
        struct stat st;

        if(stat(argv[1], &st) == 0)
        {
            if(S_ISREG(st.st_mode) == true)
            {
                return true;
            }
            else
            {
                return false;
            }
        }
        else
        {
            return false;
        }
    }
    else
    {
        return false;
    }
}
