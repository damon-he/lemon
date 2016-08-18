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
�߳����˼���
�̺߳����������
�����Thead�����󶨵�һ����ô�ͱ��뱣֤Thread����Ч��
����ǳ����һ��POD����������?
���̺߳����������ݣ�Threadֻ�з��ʵ�Ȩ��?��Thread������� �̺߳�����ô��� 
�̺߳������˺�POD����˭������?��������----�̹߳������������ָ��
++++++++����������Thread��������Ϊָ�룬����Thread�������⣬Ҳ��Ӱ���̺߳���
---Ҳ���У�ֻҪ���̹߳������ݣ���ô����������ָ�룬����Ҳ��һ���ô�
---��Thread�������������߳�һ��������˭����������һ����

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

// XXX: �ú��������������ָ�룬�������������ָ��ĸ����Thread���̺߳���
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
