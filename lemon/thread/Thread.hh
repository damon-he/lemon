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
Ïß³ÌÉè¼ÆË¼Ïëå
Ïß³Ìº¯ÊıµÄÊı¾İå
Èç¹û¸úTheadÕâ¸öÀà°ó¶¨µ½Ò»Æğ£¬ÄÇÃ´¾Í±ØĞë±£Ö¤ThreadµÄÓĞĞ§ĞÔ
Èç¹ûÊÇ³éÏó³öÒ»¸öPODÀàĞÍÊı¾İÄØ?
ÈÃÏß³Ìº¯Êı³ÖÓĞÊı¾İ£¬ThreadÖ»ÓĞ·ÃÎÊµÄÈ¨ÏŞ?ÄÇThreadËÀÁËÄØå Ïß³Ìº¯ÊıÔõÃ´°ìå 
Ïß³Ìº¯Êı¹ÒÁËºó£¬PODÊı¾İË­À´»ØÊÕ?¸÷ÖÖÎÊÌâ----Ïß³Ì¹²Ïí£¬×îºÃÓÃÖÇÄÜÖ¸Õë
++++++++¹²ÏíÊı¾İÓÉThread¹ÜÀí£¬ÉèÖÃÎªÖ¸Õë£¬ÕâÑùThread·¢ÉúÒâÍâ£¬Ò²²»Ó°ÏìÏß³Ìº¯Êı
---Ò²²»ĞĞ£¬Ö»ÒªÊÇÏß³Ì¹²ÏíÊı¾İ£¬ÄÇÃ´±ØĞëÓÃÖÇÄÜÖ¸Õë£¬ÕâÑùÒ²ÓĞÒ»¸öºÃ´¦
---¼´ThreadµÄÉú´æÖÜÆÚÓëÏß³ÌÒ»Ñù£¬ÎŞÂÛË­ÏÈËÀ£¬¶¼»áÒ»ÆğËÀ

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

// XXX: ÓÃº¯Êı¶ÔÏóÌæ´úº¯ÊıÖ¸Õë£¬Æä´ÎÊÇÒıÈëÖÇÄÜÖ¸ÕëµÄ¸ÅÄî£¬°ó¶¨ThreadÓëÏß³Ìº¯Êı
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
