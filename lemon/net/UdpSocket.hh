/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      UdpSocket.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */


#ifndef LEMON_UDP_SOCKET_H
#define LEMON_UDP_SOCKET_H

#include "Socket.hh"

namespace lemon{

class UdpSocket : public Socket {
public:
	UdpSocket(bool block = true) 
		: Socket(false, block) {

	}

	~UdpSocket() {

	}
	
	virtual size_t write(const unsigned char *data, const size_t dataLen, const SocketAddress &socketAddress = 0);
	virtual size_t read(unsigned char *const buff, const size_t buffSize, SocketAddress *socketAddress = NULL);	
	
};

}

#endif
