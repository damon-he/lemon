/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Socket.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */

#ifndef LEMON_SOCKET_HH
#define LEMON_SOCKET_HH

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>			
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <cstddef>
#include "LemonCommon.hh"

namespace lemon {

const size_t IP_MAX_LEN  = 16;

class SocketAddress {
public:
	SocketAddress(const char *ip = NULL, unsigned short port = 0) 
		: _port(port) {
		if (NULL == ip) {
			strcpy(_ip, "0.0.0.0");
		} else {
			strcpy(_ip, ip);
		}
	}
	
	void setPort(const unsigned short port) {
		_port = port;
	}
	
	const unsigned short getPort() const  {
		return _port;
	}
	
	void setIP(const char *const ip) {
		if (ip != NULL) {
			strcpy(_ip, ip);
		}
	}
	
	const char *getIP() const {
		return &_ip[0];
	}
	
	SocketAddress &operator=(const SocketAddress &rhs) {
		setPort(rhs.getPort());
		setIP(rhs.getIP());
		
		return *this;//for a=b=c
	}

	~SocketAddress() {

	}
	
private:
	char _ip[IP_MAX_LEN];
	unsigned short _port;
};

class Socket {
public:
	Socket(const bool tcp, const bool block = true);
	Socket(const int fd, SocketAddress socketAddress, const bool tcp, 
		   const bool block = true);
	virtual ~Socket();

	int bind(const SocketAddress &socketAddress, const bool reuseAddress = true);
	int connect(const SocketAddress &socketAddress);
	virtual size_t write(const unsigned char *data, const size_t dataLen, 
		                 const SocketAddress &socketAddress = 0) = 0;
	virtual size_t read(unsigned char *const buff, const size_t buffSize, 
		                SocketAddress *socketAddress = NULL) = 0;

	bool isBlock() const {
		return _block;
	}
	
	int isTcp()  const {
		return _tcp;
	}
	
	int getFd()  const {
		return _fd;
	}

	const SocketAddress &getSocketAddress() const {
		return _socketAddress;
	}
	
	static void socketAddressToInetAddress(const SocketAddress &socketAddress, 
		                                   struct sockaddr_in *inetAddress);
	static void inetAddressToSocketAddress(const struct sockaddr_in *inetAddress, 
		                                   SocketAddress &socketAddress);

private:
	DISALLOW_COPY_AND_ASSIGN(Socket);
	
	int _fd;
	bool _tcp;
	bool _block;
	SocketAddress _socketAddress;
	static SocketAddress defaultAddress;
};

}
#endif

