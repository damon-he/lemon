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

void *Thread::ThreadLoop(void *arg) {
	Thread *thread = static_cast<Thread *>(arg);
	
	thread->callThreadFun();
	
	return (void *)0;
}

Thread::Thread (const ThreadFunc &threadFunc)
  : _started(false),
  	_pthreadId(0),
  	_threadFunc(threadFunc) {

}

Thread::~Thread() {
	if (_started) {
		join();
	}
}

bool Thread::start() {
 	CHECK_BOOL_VAL(!_started, false);
  	
	if (pthread_create(&_pthreadId, NULL, Thread::ThreadLoop, this)) {
	    return _started;		
  	} 
	_started = true; 

	return _started;
}

void Thread::join() {
	CHECK_BOOL(_started);
	
  	pthread_join(_pthreadId, NULL);
	_started = false;
}

void Thread::callThreadFun() const {
	_threadFunc();
}

