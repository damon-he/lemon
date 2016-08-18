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
#include "Thread.hh"
#include "Mutex.hh"
#include "Condition.hh"

using namespace std;

namespace lemon {
// XXX:用函数对象替代函数指针
// XXX:Thread使用前置声明，声明为指针，减少编译依赖
typedef void (*TaskFunc)(void *data);

struct Task {
	TaskFunc _func;
	void *_data;
};


class ThreadPool {
public:
	ThreadPool();
	~ThreadPool();

	void start(int32_t numThreads);
	void addTask(TaskFunc func, void *data);
	void stop();

	bool _running;
  	mutable Mutex _mutex;
  	Condition _notEmpty;
  	//Condition notFull_;
  	//size_t _maxQueueSize;
 	vector<Thread *> _threads;
	deque<Task *> _taskQueue;
private:
#if 0
	bool _running;
  	mutable Mutex _mutex;
  	Condition _notEmpty;
  	//Condition notFull_;
  	//size_t _maxQueueSize;
 	vector<Thread *> _threads;
	deque<Task *> _taskQueue;
#endif  
};



}

#endif