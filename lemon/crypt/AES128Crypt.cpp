/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      AES128Crypt.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/13
 */
#include "LemonCommon.hh"

extern "C" {
#include "aes.h"
}
#include "AES128Crypt.hh"

using namespace lemon;

DataBuff &AES128Crypt::encryptCBC(unsigned char *input, unsigned int len) {
	AES128_CBC_encrypt_buffer(_outDataBuff.data, input, len, cryptKey, cryptIv);
	_outDataBuff.len = (len + AES128_BLANK_SIZE -1) & ~(AES128_BLANK_SIZE - 1);
	return _outDataBuff;
}


DataBuff &AES128Crypt::decryptCBC(unsigned char *input, unsigned int len) {
	AES128_CBC_decrypt_buffer(_outDataBuff.data, input, len, cryptKey, cryptIv);
	_outDataBuff.len = len; 
	return _outDataBuff;
}


