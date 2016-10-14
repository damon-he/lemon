/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Thread.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */

#ifndef LEMON_THREAD_HH
#define LEMON_THREAD_HH

#include <pthread.h>
#include <boost/function.hpp>
#ifdef IOS
#include <sys/_types/_pid_t.h>
#endif
#include "LemonCommon.hh"



namespace CurrentThread {
	extern __thread int32_t _cachedTid;
  	extern __thread char _tidString[32];
  	extern __thread int _tidStringLen;
	void cacheTid();
	
	inline int32_t tid(){
		if (__builtin_expect(_cachedTid == 0, 0)) {
			cacheTid();
		}
		return _cachedTid;
	}
	
	inline const char* tidString() {
		cacheTid();
	 	return _tidString;
	}
	
	inline int tidStringLen() {
		cacheTid();
	 	return _tidStringLen;
	}
	
	pid_t getTid();
	
	bool isMainThread();
}

namespace lemon {


typedef boost::function<void ()> ThreadFunc;


class Thread {
public:	
	explicit Thread(const ThreadFunc &threadFunc);
	~Thread();
	
	bool start();
	void join();
	
	
private:
	static void *ThreadLoop(void *arg);

	void callThreadFun() const;
	
	bool _started;
	pthread_t _pthreadId;
	const ThreadFunc _threadFunc;
	DISALLOW_COPY_AND_ASSIGN(Thread);
};



}

#endif
