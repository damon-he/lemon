/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      TcpSocket.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */


#ifndef LEMON_TCP_SOCKET_H
#define LEMON_TCP_SOCKET_H

#include "Socket.hh"

namespace lemon {

class TcpSocket : public Socket {
public:
	TcpSocket(bool block = true) 
		: Socket(true, block) {

	}
	
	TcpSocket(const int fd, SocketAddress &address, bool block = false) 
		: Socket(fd, address, true, block) {

	}

	~TcpSocket() {

	}
	
	virtual size_t write(const unsigned char *data, const size_t dataLen, const SocketAddress &sockerAddress = 0);
	virtual size_t read(unsigned char *const buff, const size_t buffSize, SocketAddress *sockerAddress = 0);

	int listen(unsigned int maxClients);
	int accept(TcpSocket *&clientSocket);

};

}

#endif

