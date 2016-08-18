/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      LogTest.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/14
 */


#include "Log.hh"

using namespace  lemon;

#include <unistd.h>

int main(int argc, char *argv[]) {
	int a = 5;
	unsigned int b = 4;
//	float c = 9.12;	
//	LogManager::setSync(false);
	LogManager::setLogFilePath(argv[0]);
	int c = 34;
	int d = 31;	
	int e = 12;
	unsigned char df = 1;
	LEMON_INFO("lemon df=%c\n", df);		
	LEMON_DEBUG("HELLO world\n");
	LEMON_INFO("HELLO world %d\n", a);
	LEMON_WARN("HELLO world %05d\n", a);
	LEMON_ERROR("HELLO world a=%d b=%04d\n", a, b);
	LEMON_ERROR("HELLO world a=%d b=%d c=%d(%d) d=%d\n", a, b, c,e, d);
	while(1) {
		sleep(10);
	}
	return 0;
}
