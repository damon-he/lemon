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
 
#include <boost/bind.hpp>
#include "LemonCommon.hh"
#include "Thread.hh"
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

void ThreadPool::ThreadPoolLoop(void *arg) {
	ThreadPool *threadPool = static_cast<ThreadPool *>(arg);
	
	while (threadPool->isRunning()) {	
		threadPool->waitForTask();
		threadPool->excuteTask();

	}
}

bool ThreadPool::start(int numThreads) {
	CHECK_BOOL_VAL(!_running, false);
	
	if (numThreads <= 0) {
		numThreads = 1;
	}
	
	for (int i = 0; i < numThreads; i++) {
		boost::shared_ptr<Thread> thread(new Thread(boost::bind(ThreadPool::ThreadPoolLoop, this)));
		_threads.push_back(thread);
		_threads[i]->start();
	}
	_running = true;

	return _running;
}

void ThreadPool::addTask(const TaskFunc &taskFunc) {
	MutexLock lock(_mutex);
	
	_taskQueue.push_back(taskFunc);
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
		
	}	

}

void ThreadPool::waitForTask() {
	if (_taskQueue.empty()) {
		_notEmpty.wait();
	}		
}

void ThreadPool::excuteTask() {
	MutexLock lock(_mutex);
	
	if (_running && !_taskQueue.empty()) {
		TaskFunc &taskFunc = _taskQueue.front();
		taskFunc();
		_taskQueue.pop_front();			
	}		
}
 