/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      ThreadPool.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/11
 */
 
#ifndef LEMON_THREADPOOL_HH
#define LEMON_THREADPOOL_HH

#include <vector>
#include <deque>
#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include "LemonCommon.hh"
#include "Mutex.hh"
#include "Condition.hh"

using namespace std;

namespace lemon {

class Thread;

typedef boost::function<void ()> TaskFunc;

class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();

	bool start(int numThreads = 1);
	void addTask(const TaskFunc &taskFunc);
	void stop();

private:
	static void ThreadPoolLoop(void *arg); 
	DISALLOW_COPY_AND_ASSIGN(ThreadPool);

	bool isRunning() {
		return _running;
	}
	void waitForTask();
	void excuteTask();

	bool _running;
	Mutex _mutex;
  	Condition _notEmpty;
 	vector<boost::shared_ptr<Thread> > _threads;
	deque<TaskFunc> _taskQueue;
};


}

#endif