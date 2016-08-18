/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Timer.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */

#ifndef LEMON_TIMER_HH
#define LEMON_TIMER_HH

#include <pthread.h>
#include <vector>
#include "Mutex.hh"
#include "Condition.hh"

using namespace std;

namespace lemon {

typedef void (*TimerCallBack)(void *data);


typedef enum {
	OneShootTimer,
	RepeatTimer
} TimerType;


class Timer {
public:
	Timer(TimerType type, int sec, int msec, TimerCallBack callBack, void *data);
	~Timer();	
	
	void updateExpire();
	
	int getTimeout();
	
	bool operator<(Timer &node);

	TimerType getTimerType() {
		return _type;
	}
	
	void *_data;
	TimerCallBack _callBack;
	unsigned int _expireSec;
	unsigned int _expireMSec;	
private:

	TimerType _type;
	unsigned int _sec;
	unsigned int _msec;

};

class MinHeap {
public:
	MinHeap() {}
	
	~MinHeap() {}
	
	bool empty() {
		return _heap.empty();
	}
	
	int size() {
		return _heap.size();
	}
	
	Timer *front() {
		return _heap.front();
	}
	
	void push(Timer *timer);

	Timer *pop();

private:
	vector<Timer *> _heap;
};

class TimerManager {
public:
	TimerManager(bool threadMode = false);
	~TimerManager();
	
	void addTimer(Timer *timer);
	int getMinTimeout();
	bool empty();
	void doAction();

	static void *timerThread(void *arg);
	
private:  	
	MinHeap _minHeap;
	bool _threadMode;
	bool _running;
	Mutex _minHeapMutex;
	Condition _minHeapCond;
	pthread_t _timerPthreadt;
	
};



}


#endif

