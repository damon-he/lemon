/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Condition.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */
 
#ifndef LEMON_CONDITION_HH
#define LEMON_CONDITION_HH

#include <sys/time.h>
#include "Mutex.hh"

namespace lemon {

class Condition {
public:
	explicit Condition(Mutex &mutex) 
		: _mutex(mutex) {
		pthread_cond_init(&_cond, NULL);
	}
	
	~Condition() {
		pthread_cond_destroy(&_cond);
	}
	
	void wait() {
   		pthread_cond_wait(&_cond, const_cast<pthread_mutex_t *>(_mutex.getMutexImpl()));
  	}

	void timeWait(int timeMsec) {
		struct timeval now;
  		struct timespec outTime;
		gettimeofday(&now, NULL);
    	outTime.tv_sec = now.tv_sec + timeMsec/1000;
    	outTime.tv_nsec = (now.tv_usec + timeMsec%1000*1000)* 1000;
    	pthread_cond_timedwait(&_cond, const_cast<pthread_mutex_t *>(_mutex.getMutexImpl()), &outTime);
	}
	
	void notify() {
	    pthread_cond_signal(&_cond);
	}

	void notifyAll() {
	    pthread_cond_broadcast(&_cond);
	}	
	
private:
	Mutex &_mutex;
	pthread_cond_t _cond;
	DISALLOW_COPY_AND_ASSIGN(Condition);
};

}



#endif