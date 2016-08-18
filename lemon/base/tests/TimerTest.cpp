/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      TimerTest.cpp
 * \author    Damon He   
 * \version   1.0
 * \date      2016/7/12
 */


#include <sys/time.h>
#include <unistd.h>
#include <iostream>
#include "Timer.hh"

using namespace std;
using namespace lemon;

void timerFun(void *data) {
	struct timeval tm;
	gettimeofday(&tm, NULL);

	cout<<"Timer ID="<<*((int *)data)<<" time="<<tm.tv_sec<<":"<<tm.tv_usec/1000<<endl;
}


void TimerTest() {
	TimerManager manager(true);

	struct timeval tm;
	gettimeofday(&tm, NULL);

	cout<<"ret_time="<<tm.tv_sec<<":"<<tm.tv_usec/1000<<endl;

	int id1 = 1;
	Timer *node1 = new Timer(OneShootTimer, 6, 0, timerFun, &id1);
	manager.addTimer(node1);
	
	int id2 = 2;
	Timer *node2 = new Timer(OneShootTimer, 15, 0, timerFun, &id2);
	manager.addTimer(node2);

	int id3 = 3;
	Timer *node3 = new Timer(OneShootTimer, 3, 0, timerFun, &id3);
	manager.addTimer(node3);
	
	int id4 = 4;
	Timer *node4 = new Timer(OneShootTimer, 1, 0, timerFun, &id4);
	manager.addTimer(node4);
	
	int id5 = 5;
	Timer *node5 = new Timer(OneShootTimer, 9, 0, timerFun, &id5);
	manager.addTimer(node5);
	
	int id6 = 6;
	Timer *node6 = new Timer(OneShootTimer, 5, 0, timerFun, &id6);
	manager.addTimer(node6);

	sleep(18);

}


int main(void) {

	TimerTest();
	

	
	return 0;
}

