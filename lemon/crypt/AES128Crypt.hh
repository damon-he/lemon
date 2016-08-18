/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      AES128Crypt.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/13
 */

#ifndef LEMON_AES128_CRYPT_H
#define LEMON_AES128_CRYPT_H

#include <stdlib.h>

namespace lemon {

static uint8_t cryptKey[] = "#S2EAswv*sd7A%Ev";//{ 0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c };
static uint8_t cryptIv[]  = { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 };

const unsigned int CRYPT_DATA_BUFF_MAX_SIZE = 1456;

const unsigned int AES128_BLANK_SIZE = 16;


class AES128Crypt {
public:
	AES128Crypt() {
		_outDataBuff.data = (unsigned char *)malloc(CRYPT_DATA_BUFF_MAX_SIZE);
		_outDataBuff.len = 0;
		_outDataBuff.size = CRYPT_DATA_BUFF_MAX_SIZE;
	}
	
	~AES128Crypt() {
		free(_outDataBuff.data);
	}
	
	DataBuff &encryptCBC(unsigned char *input, unsigned int len);
	DataBuff &decryptCBC(unsigned char *input, unsigned int len);
	DataBuff _outDataBuff;
};

}

#endif
