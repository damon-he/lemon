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

/*
线程设计思想�
线程函数的数据�
如果跟Thead这个类绑定到一起，那么就必须保证Thread的有效性
如果是抽象出一个POD类型数据呢?
让线程函数持有数据，Thread只有访问的权限?那Thread死了呢� 线程函数怎么办� 
线程函数挂了后，POD数据谁来回收?各种问题----线程共享，最好用智能指针
++++++++共享数据由Thread管理，设置为指针，这样Thread发生意外，也不影响线程函数
---也不行，只要是线程共享数据，那么必须用智能指针，这样也有一个好处
---即Thread的生存周期与线程一样，无论谁先死，都会一起死

*/
#ifndef LEMON_THREAD_HH
#define LEMON_THREAD_HH

#include <stdint.h>
#include <pthread.h>
#include "Mutex.hh"
#ifdef IOS
#include <sys/_types/_pid_t.h>
#endif

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

typedef void (*ThreadFunc)(void *data);

struct ThreadData {
	pid_t _tid;
	ThreadFunc _func;
	void *_data;
};

// XXX: 用函数对象替代函数指针，其次是引入智能指针的概念，绑定Thread与线程函数
class Thread {
public:	
	Thread(ThreadFunc func, void *data);
	~Thread();
	void start();
	int32_t join();
	
private:
	bool _started;
	bool _joined;
	pthread_t _pthreadId;
	ThreadData *_threadData;
};



}

#endif
