/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      ThreadTest.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */

#include <unistd.h>
#include <iostream>
#include <boost/bind.hpp>

#include "LemonCommon.hh"
#include "Thread.hh"
#include "Condition.hh"

using namespace std;
using namespace lemon;

#include <unistd.h>

Mutex testMutex;

void ThreadLock(void *arg) {
	int32_t *num = (int32_t *)arg;
	int32_t i = 0;

	for (i = 0; i < 50000; i++) {
		MutexLock lock(testMutex);
		(*num)++;
		//usleep(1000);
	}
}





void ThreadLockTest() {
	int32_t num = 0;
	int a = 14;
	int b = 14;
	cout<<"**********ThreadLockTest start**************"<<endl;
	Thread thread1(boost::bind(ThreadLock,&num));
	Thread thread2(boost::bind(ThreadLock,&num));
	Thread thread3(boost::bind(ThreadLock,&num));

	thread1.start();
	thread2.start();
	thread3.start();

	thread1.join();
	thread2.join();
	thread3.join();
	cout<<"**********ThreadLockTest end**************"<<endl;
	cout<<"num="<<num<<endl;
}

struct ConditionData {
	Mutex _mutex;
	Condition *_cond;
	bool _var;
};


void ThreadCondition1(void *arg) {
	ConditionData *data = (ConditionData *)arg;
	int32_t i = 0;

	for (i = 0; i < 15; i++) {
		if (i%3 == 0) {
			MutexLock lock(data->_mutex);
			if (!data->_var) {
				data->_var = true;
				cout<<"nodify ThreadCondition2"<<endl;
				data->_cond->notify();
			}
		}
		sleep(1);
	}
}

void ThreadCondition2(void *arg) {
	ConditionData *data = (ConditionData *)arg;

	while (1) {
		MutexLock lock(data->_mutex);
		if (!data->_var) {
			cout<<"ThreadCondition2 wait start"<<endl;
			data->_cond->wait();
			cout<<"ThreadCondition2 wait end"<<" var="<<data->_var<<endl;
		} else {
			data->_var = false;
		}
		cout<<"ThreadCondition2 loop"<<endl;
			
	}

}
void ThreadConditionTest() {
	ConditionData data;
	data._cond = new Condition(data._mutex);
	data._var = false;
	
	Thread thread1(boost::bind(ThreadCondition1, &data));
	Thread thread2(boost::bind(ThreadCondition2, &data));	
	thread1.start();
	thread2.start();

	thread1.join();
	thread2.join();
}


#include "ThreadPool.hh"
int main(int agrc, char *argv[]) {
	//ThreadLockTest();
	//ThreadConditionTest();

	ThreadPool threadPool;
	threadPool.start(100);
	//threadPool-

	return 0;
}
 
