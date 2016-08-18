/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      Log.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/14
 */

#include <iostream>

#ifdef ANDROID
#include <android/log.h>
#endif

#include "File.hh"
#include "Log.hh"


using namespace lemon;

Mutex LogManager::_mutex;
Log *LogManager::_log = NULL;
bool LogManager::_sync = true;  //default sync log
unsigned LogManager::_logLevel = LEMON_LOG_DEBUG;
string LogManager::_logFilePath("lemon.log");

time_t StringFormat::_lastSec = 0;
char StringFormat::_dateTimeStr[DATE_TIME_MAX_LEN] = {0};
struct tm StringFormat::_dateTimeTm = {0};



StringFormat::StringFormat(const char *srcFile, int line, unsigned logLevel, const char *fmt) 
	: _fmt(fmt), _fmtIndex(0) {
	_fmtLen = ::strlen(fmt);
		
	struct timeval curTime;
	gettimeofday(&curTime, NULL);
	if (_lastSec != curTime.tv_sec) {
		localtime_r(&curTime.tv_sec, &_dateTimeTm);
		_lastSec = curTime.tv_sec; 
	}
	snprintf(_dateTimeStr, sizeof(_dateTimeStr), "%4d%02d%02d %02d:%02d:%02d.%06ld ",
		_dateTimeTm.tm_year + 1900, _dateTimeTm.tm_mon + 1, _dateTimeTm.tm_mday,
		_dateTimeTm.tm_hour, _dateTimeTm.tm_min, _dateTimeTm.tm_sec, curTime.tv_usec);
	_result += _dateTimeStr;
	
	_result += CurrentThread::tidString();

	_result += File::getBaseName(srcFile) + ' ';
	
	string lineStr;
	_stream << line;
	_stream >> lineStr;
	_result += lineStr + ' ';

	_result += LogManager::getLogLevelName(logLevel);
	
}

StringFormat::~StringFormat() {

}



bool StringFormat::nextWildcard() {
	_type = '\0';
	_minLen = 0;
	_fillChar = ' ';
	char nextChar = '\0';
	char curChar = '\0';
	
	for (; _fmtIndex < _fmtLen; _fmtIndex++) {
		if (_fmt[_fmtIndex] != '%') {
			_result += _fmt[_fmtIndex];
			continue;
		} 

		nextChar = _fmt[_fmtIndex + 1];
		if ('%' == nextChar) {
			_fmtIndex += 1;
			_result += nextChar;
			continue;
		}

		for (int i = 1; i <= 5; i++) {
			curChar = _fmt[_fmtIndex + i];
			if ('\0' == curChar || '%' == curChar) {
				//_fmtIndex += i;
				break;
			} else if ('c' == curChar || 'd' == curChar || 'u' == curChar
				|| 'x' == curChar || 's' == curChar || 'f' == curChar) {
				_type = curChar;
				_minLen = ::atoi(_fmt + _fmtIndex + 1);
				if ('0' == nextChar) {
					_fillChar = '0';
				} 
				_fmtIndex += i + 1;
				return true;
			} else if ('l' == curChar) {
				_type = _fmt[_fmtIndex + i + 1];
				if ('d' == _type || 'u' == _type) {
					_minLen = ::atoi(_fmt + _fmtIndex + 1);
					_fmtIndex += i + 2;
					if ('0' == nextChar) {
						_fillChar = '0';
					} 
					return true;
				}

			}

		}
		
		_result += curChar;

	}
	
	return false;

}


void StringFormat::append(char data) {
	if (nextWildcard()) {
		unsigned int len = 0;
		if (_type != 'c') {
			if (data/100 != 0) {
				len = 3;
			} else if (data/10 != 0) {
				len = 2;
			} else {
				len = 1;
			}
		} else {
			len = 1;
		}
		int fillLen = _minLen > len ? _minLen -len : 0;
		for (; fillLen > 0; fillLen--) {
			_result += _fillChar;
		} 
	}
		
	if (data == '#') {
		
	} else {
		if ('c' == _type) {
			_result += data;
		} else {
			int tmpData = data;
			_stream.clear();
			_tmpBuff.clear();
			_stream << tmpData;
			_stream >> _tmpBuff;
			_result += _tmpBuff;
		}
	}


}

void StringFormat::append(unsigned char data) {
	if (nextWildcard()) {
		unsigned int len = 0;
		if (_type != 'c') {
			if (data/100 != 0) {
				len = 3;
			} else if (data/10 != 0) {
				len = 2;
			} else {
				len = 1;
			}
		} else {
			len = 1;
		}
		int fillLen = _minLen > len ? _minLen -len : 0;
		for (; fillLen > 0; fillLen--) {
			_result += _fillChar;
		} 
	}
	
	if (data == '#') {
		
	} else {
		if ('c' == _type) {
			_result += data;
		} else {
			int tmpData = data;
			_stream.clear();
			_tmpBuff.clear();
			_stream << tmpData;
			_stream >> _tmpBuff;
			_result += _tmpBuff;
		}
	}

}



void StringFormat::append(const char* str) {
	if (nextWildcard()) {
		unsigned int len = ::strlen(str);
		int fillLen = _minLen > len ? _minLen -len : 0;
		for (; fillLen > 0; fillLen--) {
			_result += _fillChar;
		} 
	}
	
	_result += str;
}

void StringFormat::append(const string &str) {
	if (nextWildcard()) {
		unsigned int len = str.length();
		int fillLen = _minLen > len ? _minLen -len : 0;
		for (; fillLen > 0; fillLen--) {
			_result += _fillChar;
		}
	}
	_result += str;

}



Log::Log(const string &filePath) 
	: _condition(_mutex) {
	_logFile = new File(filePath);
	_logFile->open(OPEN_MODE_WRITE | OPEN_MODE_CREATE);
}

Log::~Log() {
	_logFile->close();
}


bool Log::outEnable(int logLevel) {
	return logLevel >= LogManager::getLogLevel();
}


SyncLog::SyncLog(const string &filePath) 
	: Log(filePath) {

}

SyncLog::~SyncLog() {

}

void SyncLog::output(const string &record) {
	MutexLock lock(_mutex);
#ifdef ANDROID	
	__android_log_print(ANDROID_LOG_INFO, "lemon", record.c_str());
#else
	std::cout<<record;
#endif
	_logFile->write(reinterpret_cast<const unsigned char *>(record.c_str()),record.length());
	_logFile->ffulsh();
}


AsyncLog::AsyncLog(const string &filePath) 
	: Log(filePath) {

	DataBuff *dataBuff = NULL;
	
	for (int i = 0; i < 4; i++) {
		dataBuff =new DataBuff();
		dataBuff->data = (unsigned char *)malloc(100*1024);
		dataBuff->len = 0;
		dataBuff->size = 100*1024;
		_freeBuffs.push_back(dataBuff);
	}
	
	_running = true;
	pthread_create(&_asyncThreadT,  NULL, AsyncThread, this);	
	
}

AsyncLog::~AsyncLog() {
	_running = false;
	pthread_join(_asyncThreadT, NULL);
}


void AsyncLog::output(const string &record) {
	MutexLock lock(_mutex);
	DataBuff *dataBuff = _freeBuffs.front();

	if (dataBuff->len + record.length() <= dataBuff->size) {
		memcpy(dataBuff->data + dataBuff->len, record.c_str(), record.length());
		dataBuff->len += record.length();
	} else {
		_freeBuffs.pop_front();
		_usedBuffs.push_back(dataBuff);
		if (_freeBuffs.empty()) {
			dataBuff =new DataBuff();
			dataBuff->data = (unsigned char *)malloc(100*1024);
			dataBuff->len = 0;
			dataBuff->size = 100*1024;
			_freeBuffs.push_back(dataBuff);
		}
		dataBuff = _freeBuffs.front();
		memcpy(dataBuff->data, record.c_str(), record.length());
		dataBuff->len += record.length();
	}
}

void *AsyncLog::AsyncThread(void *arg) {
	AsyncLog *async = (AsyncLog *)arg;
	DataBuff *dataBuff = NULL;
	
	while(async->_running) {
		MutexLock lock(async->_mutex);
		if (async->_usedBuffs.empty()) {
			async->_condition.timeWait(3000);
		}
		dataBuff = async->_freeBuffs.front();
		if (dataBuff->len > 0) {
			async->_freeBuffs.pop_front();
			async->_usedBuffs.push_back(dataBuff);
		}
		for (list<DataBuff *>::iterator it = async->_usedBuffs.begin(); it != async->_usedBuffs.end(); ++it) {

#ifdef ANDROID	
			__android_log_print(ANDROID_LOG_INFO, "lemon", reinterpret_cast<char *>((*it)->data));
#else
			std::cout<<(*it)->data;
#endif

			
			async->_logFile->write((*it)->data, (*it)->len);
			async->_logFile->ffulsh();
		}

		for (list<DataBuff *>::iterator it = async->_usedBuffs.begin(); it != async->_usedBuffs.end(); ++it) {
			(*it)->len = 0;
			async->_freeBuffs.push_back((*it));
    	}	
		async->_usedBuffs.clear();
		

	}
    return (void *)0;
}




const string LogManager::getLogLevelName(unsigned logLevel) {
	string logLevelName;
	
	switch (logLevel) {
		case LEMON_LOG_DEBUG : {
			logLevelName = "[DEBUG] ";
			break;
		}
		
		case LEMON_LOG_INFO : {
			logLevelName = "[INFO] ";
			break;
		}
		
		case LEMON_LOG_WARN : {
			logLevelName = "[WARN] ";
			break;
		}	
		
		case LEMON_LOG_ERROR : {
			logLevelName = "[ERROR] ";
			break;
		}	
		
		case LEMON_LOG_FATAL : {
			logLevelName = "[FATAL] ";
			break;
		}	
		
		deafult : {
			logLevelName = "[UNKNOW]";
			break;
		}			
	}

	return logLevelName;
}

