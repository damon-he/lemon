/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      ThreadPool.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */

#include "LemonCommon.hh"
#include "ThreadPool.hh"

using namespace lemon;

ThreadPool::ThreadPool() 
  : _running(false),
    _notEmpty(_mutex) {
	
}

ThreadPool::~ThreadPool() {
	if (_running) {
		stop();
	}
}	

void ThreadPoolFunc(void *arg) {
	ThreadPool *threadPool = (ThreadPool *)arg;
	
	while (threadPool->_running) {
		MutexLock lock(threadPool->_mutex);
		while (threadPool->_taskQueue.empty()) {
			threadPool->_notEmpty.wait();
		}

		if (!threadPool->_taskQueue.empty()) {
			Task *task = threadPool->_taskQueue.front();
    		threadPool->_taskQueue.pop_front();
			task->_func(task->_data);
		}

	}
}

void ThreadPool::start(int32_t numThreads) {
	int32_t i = 0;
	Thread *thread = NULL;

	CHECK_BOOL(!_running);
	_running = true;

	if (numThreads <= 0) {
		numThreads = 1;
	}
	
	for (i = 0; i < numThreads; i++) {
		thread = new Thread(ThreadPoolFunc, this);
		_threads.push_back(thread);
		_threads[i]->start();
	}

}

void ThreadPool::addTask(TaskFunc func, void *data) {
	Task *task = new Task();
	task->_func = func;
	task->_data = data;
	
	MutexLock lock(_mutex);
	_taskQueue.push_back(task);
	if (1 == _taskQueue.size()) {
		_notEmpty.notify();
	}
	
}

void ThreadPool::stop() {
	if (_running) {
		_running = false;
		{
			MutexLock lock(_mutex);
			_notEmpty.notifyAll();
		}
#if 0
		//error: no match for ¡®operator=¡¯
		vector<Thread>::iterator it;
		for (it = _threads.begin(); it != _threads.end(); ++it) {
			it->join();
			//XXX: delete it, but vector is safe?
		}
#endif	
		int32_t i = 0;
		for (i = 0; i < _threads.size(); i++) {
			_threads[i]->join();
		}
		
	}	

}


 