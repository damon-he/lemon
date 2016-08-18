/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Thread.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */
 
#include <unistd.h>
#include <sys/syscall.h>   
#include "LemonCommon.hh"
#include "Thread.hh"

namespace CurrentThread {
	__thread int32_t _cachedTid = 0;
	__thread char _tidString[32];
  	__thread int _tidStringLen = 6;
	
	pid_t getTid() {
#ifdef ANDROID
	return static_cast<pid_t>(::gettid());
#else
	return static_cast<pid_t>(::syscall(SYS_gettid));
#endif
	}
	
	void cacheTid() {
		if (_cachedTid == 0) {
			_cachedTid = getTid();
			_tidStringLen = snprintf(_tidString, sizeof _tidString, "%5d ", _cachedTid);
		}
	}

	bool isMainThread() {
	  return tid() == ::getpid();
	}
}


using namespace lemon;

// XXX: why declear friend not useful ?
void *ThreadLoop(void *arg) {
	ThreadData *threadData = (ThreadData *)arg;

	threadData->_tid = CurrentThread::tid();
	
	threadData->_func(threadData->_data);
	
	return NULL;
}

Thread::Thread (ThreadFunc func, void *data)
  : _started(false),
  	_joined(false),
  	_pthreadId(0) {
  	_threadData = new ThreadData();
	_threadData->_data = data;
	_threadData->_tid = 0;
	_threadData->_func = func;

}

Thread::~Thread() {
	if (_started) {
		join();
	}
	delete _threadData;
}

void Thread::start() {
 	CHECK_BOOL(!_started);
  	_started = true;	
  	if (pthread_create(&_pthreadId, NULL, ThreadLoop, _threadData)) {
		// XXX: how deal?
	    _started = false;
  	}
}

int32_t Thread::join() {
	CHECK_BOOL_VAL(_started, LEMON_SUCCESS);
	CHECK_BOOL_VAL(!_joined, LEMON_FAILURE);
	_joined = true;
  	return pthread_join(_pthreadId, NULL);
}
