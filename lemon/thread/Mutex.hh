/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Mutex.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */
 
#ifndef LEMON_MUTEX_HH
#define LEMON_MUTEX_HH

#include <pthread.h>

namespace lemon {

class Mutex {
public:
	Mutex() { 
		pthread_mutex_init(&_mutex, NULL); 
	}

	~Mutex() { 
		pthread_mutex_destroy(&_mutex); 
	}

	void lock() { 
		pthread_mutex_lock(&_mutex); 
	}

	void unlock() { 
		pthread_mutex_unlock(&_mutex); 
	}
	
	void* getMutexImpl () {
    	return &_mutex;
  	}
	
private:
	pthread_mutex_t _mutex;
};

class MutexLock {
public:
	explicit MutexLock(Mutex &mutex) : _mutex(mutex) { 
		_mutex.lock(); 
	}
	~MutexLock() { 
		_mutex.unlock(); 
	}
	
private:
	Mutex &_mutex;
};



}

#endif