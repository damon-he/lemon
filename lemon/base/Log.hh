/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Log.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/14
 */
#ifndef LEMON_LOG_HH
#define LEMON_LOG_HH

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <string>
#include <sstream>
#include <sys/time.h>

#include <list>

#include "File.hh"
#include "Mutex.hh"
#include "Condition.hh"
#include "Thread.hh"

using namespace std;


namespace lemon {

enum {
    LEMON_LOG_DEBUG = 0,
    LEMON_LOG_INFO,
    LEMON_LOG_WARN,
    LEMON_LOG_ERROR,
    LEMON_LOG_FATAL,
};

const size_t DATE_TIME_MAX_LEN = 30;


class LogManager;

#define LOG_IMPL_NONE_ARG(func) \
	inline void func(const char *srcFile, int line, int logLevel, const char *fmt) { \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append("#"); \
			output(stringFormat.getResult());  \
		}\
	}



#define LOG_IMPL_ARG1(func) \
	template <typename ARG1> \
	inline void func(const char *srcFile, int line, int logLevel, const char *fmt, const ARG1 &arg1) { \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append(arg1); \
			stringFormat.append("#"); \
			output(stringFormat.getResult()); \
			} \
	}



#define LOG_IMPL_ARG2(func) \
	template <typename ARG1, typename ARG2>	\
	inline void func(const char *srcFile, int line, int logLevel, const char* fmt, const ARG1& arg1, const ARG2& arg2) { \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append(arg1); \
			stringFormat.append(arg2); \
			stringFormat.append("#"); \
			output(stringFormat.getResult()); \
		} \
	}

#define LOG_IMPL_ARG3(func) \
	template <typename ARG1, typename ARG2, typename ARG3> \
	inline void func(const char *srcFile, int line, int logLevel, const char* fmt, const ARG1& arg1, const ARG2& arg2, \
			  const ARG3& arg3)	{ \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append(arg1); \
			stringFormat.append(arg2); \
			stringFormat.append(arg3); \
			stringFormat.append("#"); \
			output(stringFormat.getResult()); \
		} \
	}

#define LOG_IMPL_ARG4(func) \
	template <typename ARG1, typename ARG2, typename ARG3, typename ARG4> \
	inline void func(const char *srcFile, int line, int logLevel, const char *fmt, const ARG1 &arg1, const ARG2 &arg2, \
			  const ARG3 &arg3, const ARG4 &arg4) { \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append(arg1); \
			stringFormat.append(arg2); \
			stringFormat.append(arg3); \
			stringFormat.append(arg4); \
			stringFormat.append("#"); \
			output(stringFormat.getResult()); \
		} \
	}

#define LOG_IMPL_ARG5(func) \
	template <typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5> \
	inline void func(const char *srcFile, int line, int logLevel, const char* fmt, const ARG1 &arg1, const ARG2 &arg2, \
			  const ARG3 &arg3, const ARG4 &arg4, const ARG5 &arg5) { \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append(arg1); \
			stringFormat.append(arg2); \
			stringFormat.append(arg3); \
			stringFormat.append(arg4); \
			stringFormat.append(arg5); \
			stringFormat.append("#"); \
			output(stringFormat.getResult()); \
		} \
	}

#define LOG_IMPL_ARG6(func) \
	template <typename ARG1, typename ARG2, typename ARG3, typename ARG4, typename ARG5, \
			  typename ARG6> \
	inline void func(const char *srcFile, int line, int logLevel, const char* fmt, const ARG1& arg1, const ARG2& arg2, \
			  const ARG3& arg3, const ARG4& arg4, const ARG5& arg5, const ARG6& arg6) { \
		if (outEnable(logLevel)) { \
			StringFormat stringFormat(srcFile, line, logLevel, fmt); \
			stringFormat.append(arg1); \
			stringFormat.append(arg2); \
			stringFormat.append(arg3); \
			stringFormat.append(arg4); \
			stringFormat.append(arg5); \
			stringFormat.append(arg6); \
			stringFormat.append("#"); \
			output(stringFormat.getResult()); \
		} \
	}

	
#define LOG_IMPL_MACRO(func) 	\
	LOG_IMPL_NONE_ARG(func)		\
	LOG_IMPL_ARG1(func) 		\
	LOG_IMPL_ARG2(func) 		\
	LOG_IMPL_ARG3(func) 		\
	LOG_IMPL_ARG4(func) 		\
	LOG_IMPL_ARG5(func) 		\
	LOG_IMPL_ARG6(func) 



class StringFormat {
public:
	StringFormat(const char *srcFile, int line, unsigned logLevel, const char *fmt);
	~StringFormat();

	bool nextWildcard();

	template <typename T>
	void append(T &content) {
		_stream.clear();
		_tmpBuff.clear();
		
		if (nextWildcard()) {
			_stream << content;
			_stream >> _tmpBuff;
			
			int fillLen = _minLen > _tmpBuff.length() ? _minLen - _tmpBuff.length() : 0;
			for (; fillLen >  0; fillLen--) {
				_result += _fillChar;
			}
			
		} else {
			_stream << content;
			_stream >> _tmpBuff;
		}

		_result += _tmpBuff;
	}

	void append(char data);
	void append(unsigned char data);
	void append(const char *str);
	void append(const string &str);

	const string &getResult() {
		return _result;
	}
	
private:
	stringstream _stream;
	string _result;
	string _tmpBuff;
	static time_t _lastSec;
	static struct tm _dateTimeTm;
	static char _dateTimeStr[DATE_TIME_MAX_LEN];
	const char *_fmt;
	unsigned int _fmtIndex;
	unsigned int _fmtLen;
	char _type;
	char _fillChar;
	unsigned int _minLen;
	
};



class File;



class Log {
public:
	Log(const string &filePath);
	~Log();
	bool outEnable(int logLevel);
		
	LOG_IMPL_MACRO(format);
	
	virtual void output(const string &record) = 0;
	
protected:
	File *_logFile;
	Mutex _mutex;
	Condition _condition;
};


class SyncLog : public Log {
public:
	SyncLog(const string &filePath);
	~SyncLog();

	virtual void output(const string &record);
};

class AsyncLog : public Log {
public:
	AsyncLog(const string &filePath);
	~AsyncLog();

	virtual void output(const string &record);
	static void *AsyncThread(void *arg);

	bool _running;
private:
	list<DataBuff *> _freeBuffs;
	list<DataBuff *> _usedBuffs;
	pthread_t _asyncThreadT;
	
};


class LogManager {
public:
	static Log *instance() {
		MutexLock lock(_mutex);
		
		if (NULL == _log) {
			if (_sync) {
				_log = new SyncLog(_logFilePath);
			} else {
				_log = new AsyncLog(_logFilePath);
			}
		} 

		
		return _log;
	}
	
	static void setLogLevel(unsigned logLevel) {
		_logLevel = logLevel;
	}

	static unsigned getLogLevel() {
		return _logLevel;
	}

	static void setSync(bool sync) {
		_sync = sync;
	}

	static void setLogFilePath(const char *logFilePath) {
		_logFilePath.clear();
		_logFilePath += logFilePath;
		_logFilePath += ".log";
	}
	

	static const string getLogLevelName(unsigned logLevel); 
	
	LogManager() {};
	~LogManager() {};
	
private:
	static Mutex _mutex;
	static Log *_log;
	static unsigned int _logLevel;
	static bool _sync;
	static string _logFilePath;
};


#define LEMON_DEBUG(...) 	LogManager::instance()->format(__FILE__, __LINE__, LEMON_LOG_DEBUG, ##__VA_ARGS__)
#define LEMON_INFO(...)		LogManager::instance()->format(__FILE__, __LINE__, LEMON_LOG_INFO,  ##__VA_ARGS__)
#define LEMON_WARN(...) 	LogManager::instance()->format(__FILE__, __LINE__, LEMON_LOG_WARN,  ##__VA_ARGS__)
#define LEMON_ERROR(...) 	LogManager::instance()->format(__FILE__, __LINE__, LEMON_LOG_ERROR, ##__VA_ARGS__)
#define LEMON_FATAL(...) 	LogManager::instance()->format(__FILE__, __LINE__, LEMON_LOG_FATAL, ##__VA_ARGS__)


}

#endif
