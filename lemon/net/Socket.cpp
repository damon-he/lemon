/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Socket.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */
 
#include <unistd.h>
#include <fcntl.h>

#include "Socket.hh"

using namespace lemon;


void Socket::socketAddressToInetAddress(const SocketAddress &socketAddress, 
										struct sockaddr_in *inetAddress) {
    // setup the structure
    inetAddress->sin_family = AF_INET;
    inetAddress->sin_port = htons(socketAddress.getPort());
	inet_pton(AF_INET, socketAddress.getIP(), &inetAddress->sin_addr);
}

void Socket::inetAddressToSocketAddress(const struct sockaddr_in *inetAddress,
                           SocketAddress &socketAddress) {
    socketAddress.setPort(ntohs(inetAddress->sin_port));
	inet_ntop(AF_INET, &inetAddress->sin_addr, const_cast<char *>(socketAddress.getIP()), IP_MAX_LEN);
}

Socket::Socket(const bool tcp, const bool block) 
	: _tcp(tcp), _block(block) {
	int type = 0;
	
	
	if (tcp) {
		type = SOCK_STREAM;
	} else {
		type = SOCK_DGRAM;
	}
	_fd = socket(AF_INET, type, 0);

	int flags = fcntl(_fd, F_GETFL, 0);
    if (block) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }
	fcntl(_fd, F_SETFL, flags);
}

Socket::Socket(const int fd, SocketAddress socketAddress, const bool tcp, 
	           const bool block) : _fd(fd), _tcp(tcp), _block(block) {

	int flags = fcntl(_fd, F_GETFL, 0);
    if (block) {
        flags &= ~O_NONBLOCK;
    } else {
        flags |= O_NONBLOCK;
    }
    fcntl(_fd, F_SETFL, flags);
	
	_socketAddress = socketAddress;
}

Socket::~Socket() {
	::close(_fd);
}


int Socket::bind(const SocketAddress &socketAddress, const bool reuseAddress) {

	if (reuseAddress) {
    	int optval = 1;
    	setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, (void *)&optval, sizeof(optval));
	}
   
    // convert the address
    struct sockaddr_in inetAddress;
    socketAddressToInetAddress(socketAddress, &inetAddress);
    
    // bind the socket
    if (::bind(_fd, (struct sockaddr *)&inetAddress, sizeof(inetAddress)) < 0) {
		return LEMON_FAILURE;
    }
	_socketAddress = socketAddress;
	
  	return LEMON_SUCCESS;
}

int Socket::connect(const SocketAddress &socketAddress) {
    struct sockaddr_in inetAddress;
    socketAddressToInetAddress(socketAddress, &inetAddress);

    if (::connect(getFd(), (struct sockaddr *)&inetAddress, sizeof(inetAddress)) != 0) {
		return LEMON_FAILURE;
	}

	return LEMON_SUCCESS;
}
