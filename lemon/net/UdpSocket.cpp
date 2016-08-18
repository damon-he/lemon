/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      UdpSocket.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */


#include "UdpSocket.hh"

using namespace lemon;

size_t UdpSocket::write(const unsigned char *data, const size_t dataLen, const SocketAddress &socketAddress) {	
	size_t ret = 0;
	struct sockaddr_in inetAddress;

	Socket::socketAddressToInetAddress(socketAddress, &inetAddress);
        
    ret = sendto(getFd(), data, dataLen, 0, 
				 (struct sockaddr *)&inetAddress, 
                 sizeof(inetAddress)); 		
	
	
	return ret;
}

size_t UdpSocket::read(unsigned char *const buff, const size_t buffSize, SocketAddress *socketAddress) {
	size_t ret = 0;
    struct sockaddr_in inetAddress;
    socklen_t inetAddressLen = sizeof(inetAddress);


	ret = recvfrom(getFd(), buff, buffSize, 0, 
					(struct sockaddr *)&inetAddress, 
                    &inetAddressLen);

	Socket::inetAddressToSocketAddress(&inetAddress, *socketAddress);

	return ret;
}

