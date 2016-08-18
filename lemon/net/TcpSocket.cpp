/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      TcpSocket.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */

#include "TcpSocket.hh"

using namespace lemon;

size_t TcpSocket::write(const unsigned char *data, const size_t dataLen, const SocketAddress &) {
	size_t ret = 0;

	ret = ::write(getFd(), data, dataLen);

	return ret;
}

size_t TcpSocket::read(unsigned char *const buff, const size_t buffSize, SocketAddress *) {
	size_t ret = 0;

	ret = ::read(getFd(), buff, buffSize);

	return ret;
}

int TcpSocket::listen(unsigned int maxClients) {
    if (::listen(getFd(), maxClients) < 0) {
        return LEMON_FAILURE;
    }   

    return LEMON_SUCCESS;
}


int TcpSocket::accept(TcpSocket *&clientSocket) {
	struct sockaddr_in inetAddress;
    socklen_t inetAddressLen = sizeof(inetAddress);
	
	int clientFd = ::accept(getFd(), (struct sockaddr *)&inetAddress, &inetAddressLen); 
	if (clientFd > 0) {
		SocketAddress socketaddress;
		Socket::inetAddressToSocketAddress(&inetAddress, socketaddress);
		clientSocket = new TcpSocket(clientFd, socketaddress, isBlock());
		return LEMON_SUCCESS;
	} else {
		return LEMON_FAILURE;
	}
	
}
