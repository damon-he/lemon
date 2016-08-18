/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Timer.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 */
 
#include <sys/time.h>
#include <unistd.h>
#include <stdio.h>
#include "Timer.hh"

using namespace lemon;

Timer::Timer(TimerType type, int sec, int msec, TimerCallBack callBack, void *data) 
	: _type(type), _sec(sec), _msec(msec), _data(data), _callBack(callBack) {

}
	
void Timer::updateExpire() {
	struct timeval tm;
	
	gettimeofday(&tm, NULL);
	
	_expireMSec = (tm.tv_usec/1000 + _msec)%1000;
	_expireSec = tm.tv_sec + _sec  + (tm.tv_usec/1000 + _msec)/1000;
	
}
	
int Timer::getTimeout() {
	int timeout = 0;
	struct timeval tm;
	
	gettimeofday(&tm, NULL);

	timeout = (_expireSec - tm.tv_sec)*1000 + (_expireMSec - tm.tv_usec/1000);
	if (timeout < 0)
		timeout = 0;
	
	return timeout;
}
	
Timer::~Timer() {}
	
bool Timer::operator<(Timer &node) {
	bool ret = true;
	
	if (this->_expireSec == node._expireSec) {
		if (this->_expireMSec > node._expireMSec) {
			ret = false;
		}		
	} else if (this->_expireSec > node._expireSec){
		ret = false;
	}
	
	return ret;
}


void MinHeap::push(Timer *timer) {
	unsigned int index  = 0;
	unsigned int parent = 0;

	_heap.push_back(timer);
	index = _heap.size() - 1;
	
	while (index > 0) {
		parent = (index - 1)/2;	
		if (*_heap[parent] < *timer) { 
			break;
		}	
		_heap[index] = _heap[parent];
		index = parent;
	}
	_heap[index] = timer;
}

Timer *MinHeap::pop() { 
	unsigned int parent = 0;
	unsigned int right  = 0;
	unsigned int left   = 0;
	Timer *ret = NULL;
	
	if (_heap.empty()) {
		return ret;
	}
	ret = _heap.front();
	
	Timer *timer = _heap.back();
	_heap.pop_back();
	unsigned int half = _heap.size()/2;
	
	while (parent < half) {
		left = 2*parent + 1;
		right = left + 1;
		if (right < _heap.size()) {
			left = *_heap[left] < *_heap[right] ? left : right;
		}

		if (*timer < *_heap[left]) {
			break;
		}
		
		_heap[parent] = _heap[left];
		parent = left;

	}
	
	_heap[parent] = timer;

	return ret;
} 

TimerManager::TimerManager(bool threadMode) 
	: _threadMode(threadMode), _minHeapCond(_minHeapMutex) {
	if (threadMode) {
		_running =  true;
		pthread_create(&_timerPthreadt, NULL, TimerManager::timerThread, this);
	}
	
}

TimerManager::~TimerManager() {
	if (_threadMode) {
		_running= false;
		pthread_join(_timerPthreadt, NULL);
	}
}

void TimerManager::addTimer(Timer *timer) {
	timer->updateExpire();
	
	if (_threadMode) {
		{
			MutexLock lock(_minHeapMutex);
			_minHeap.push(timer);
		}
		if (1 == _minHeap.size()) {
			_minHeapCond.notify();
		}
	} else {
		_minHeap.push(timer);
	}
	
}

int TimerManager::getMinTimeout() {
	int ret = -1;
	Timer *timer = NULL;

	if (_threadMode) {
		MutexLock lock(_minHeapMutex);
		if (!_minHeap.empty()) {
			timer = _minHeap.front();
		}
	} else {
		if (!_minHeap.empty()) {
			timer = _minHeap.front();
		}
	}

	if (timer != NULL) {
		ret = timer->getTimeout();
	}
	
	return ret; 
}
	
bool TimerManager::empty() {
	return	_minHeap.empty();
}

void TimerManager::doAction() {
	Timer *timer = NULL;
	
	if (_threadMode) {
		MutexLock lock(_minHeapMutex);
		timer = _minHeap.pop();
	} else {
		timer = _minHeap.pop();
	}
	
	if (timer != NULL) {
		if (timer->_callBack) {
			(*timer->_callBack)(timer->_data);
		}
		if (RepeatTimer == timer->getTimerType()) {
			addTimer(timer);
		} else {
			delete timer;
		}
	}
}

void *TimerManager::timerThread(void *arg) {
	TimerManager *manager = (TimerManager *)arg;
	int minTimeOut = 0;
	
	while (manager->_running) {
		{
			MutexLock lock(manager->_minHeapMutex);
			if (manager->empty()) {
				manager->_minHeapCond.timeWait(10);
				continue;
			}
		}
		minTimeOut = manager->getMinTimeout();
		usleep(minTimeOut);
		manager->doAction();
	}
	
	return (void *)0;
}



