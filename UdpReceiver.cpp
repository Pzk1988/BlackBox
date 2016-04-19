/*
 * udpReceiver.cpp
 *
 *  Created on: Apr 18, 2016
 *      Author: pk
 */

#include "UdpReceiver.h"

UdpReceiver::UdpReceiver()
{
	port = 0;
	sck = 0;
	adrSize= 0;
	memset(&myaddr, 0x00, sizeof(sockaddr_in));
}

UdpReceiver::~UdpReceiver() {
	// TODO Auto-generated destructor stub
}

int UdpReceiver::init(int port)
{
	sck = socket(AF_INET, SOCK_DGRAM, 0);

	if(sck != -1)
	{
		memset((char *)&myaddr, 0, sizeof(myaddr));
		myaddr.sin_family = AF_INET;
		myaddr.sin_addr.s_addr = htonl(INADDR_ANY);
		myaddr.sin_port = htons(port);
		adrSize = sizeof(myaddr);

		if (bind(sck, (struct sockaddr *)&myaddr, sizeof(myaddr)) < 0)
		{
			Logger::getInstance()->log(lError, "Could not bind to port %d, %s", port, strerror(errno));
			return -1;
		}
		else
		{
			return 0;
		}
	}
	else
	{
		Logger::getInstance()->log(lError, "Could not create socket, %s", strerror(errno));
		return -1;
	}
}

int UdpReceiver::receiver(uint8_t *buff)
{
	return recvfrom(sck, buff, MAX_MSG_LEN, 0, (struct sockaddr *)&myaddr, &adrSize);
}
