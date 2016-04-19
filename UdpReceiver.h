/*
 * udpReceiver.h
 *
 *  Created on: Apr 18, 2016
 *      Author: pk
 */

#ifndef UDPRECEIVER_H_
#define UDPRECEIVER_H_

#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <inttypes.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include "global.h"
#include "Logger.h"

class UdpReceiver
{
public:
	UdpReceiver();
	int init(int port);
	int receiver(uint8_t *buff);
	virtual ~UdpReceiver();
private:
	int port;
	int sck;
	socklen_t adrSize;
	struct sockaddr_in myaddr;
};

#endif /* UDPRECEIVER_H_ */
