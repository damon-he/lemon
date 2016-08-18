/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      File.cpp
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 * \note      Our operations on files ignore the permission.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <iostream>
#include "File.hh"

using namespace lemon;

File::File(const string &path) : _path(path), _fp(NULL) {
	if (_path.compare(0, 1, "/") != 0) {
		if (File::isExist(_path)) {
			char buff[FILE_PATH_MAX_LEN] = {0};
			if (realpath(_path.c_str(), buff) != NULL) {
				_path = buff;//update _path to absolute path
			}
		} else {
			if (0 == _path.compare(0, 2, "./")) {
				//if _path contain "./" then remove it
				_path = _path.substr(2, path.length() - 2);
			}
		}
	}
}

File::~File() {
	if (_fp != NULL) {
		close();
	}
}

// XXX: may be wo can add a param 'type' to distinguish std file and dir file.
bool File::isExist(const string &path) {
	return !::access(path.c_str(), F_OK);
}

FileType File::getType(const string &path) {
	struct stat statBuff;
	FileType type = FILE_TYPE_NONE;
	
	if (::stat(path.c_str(), &statBuff) != 0) {
		return type;
	}
    if (S_ISREG(statBuff.st_mode)) {
        type = FILE_TYPE_REGULAR;
    } else if (S_ISDIR(statBuff.st_mode)) {
        type = FILE_TYPE_DIRECTORY;
    } else {
        type = FILE_TYPE_OTHER;
    }

	return type;
}

size_t File::getSize(const string &path) {
    FileType type = File::getType(path);
	size_t size = 0;
	
    if (FILE_TYPE_DIRECTORY == type) {
		DIR *directory = ::opendir(path.c_str());
		struct dirent *entryPointer = NULL;
	    while (1) {
	        entryPointer = ::readdir(directory);
	        if (NULL == entryPointer) {
				break;
	        }
	        // ignore odd names
	        if (entryPointer->d_name[0] == '\0') {
				continue;
	        }
	        // ignore . and ..
	        if (entryPointer->d_name[0] == '.' && 
	            entryPointer->d_name[1] == '\0') {
	            continue;
	        }
	        if (entryPointer->d_name[0] == '.' && 
	            entryPointer->d_name[1] == '.' &&
	            entryPointer->d_name[2] == '\0') {
	            continue;
	        } 
			size++;
	    }
    } else {
        struct stat statBuff;
		if (::stat(path.c_str(), &statBuff) != 0) {
			return size;
		}	
		size = statBuff.st_size;
	} 

	return size;
}

void File::remove(const string &path) {
    FileType type = File::getType(path);
	
	if (FILE_TYPE_DIRECTORY == type) {
		removeDir(path);
	} else {
		::remove(path.c_str());
	}
}

void File::removeDir(const string &path) {
	DIR *directory = ::opendir(path.c_str());
	struct dirent *entryPointer = NULL;

	while (1) {
		entryPointer = ::readdir(directory);
		if (NULL == entryPointer) {
			break;
		}
		// ignore odd names
		if (entryPointer->d_name[0] == '\0') {
			continue;
		}
		// ignore . and ..
		if (entryPointer->d_name[0] == '.' && 
		    entryPointer->d_name[1] == '\0') {
		    continue;
		}
		if (entryPointer->d_name[0] == '.' && 
		    entryPointer->d_name[1] == '.' &&
		    entryPointer->d_name[2] == '\0') {
		    continue;
		} 
		File::remove(path + "/" + entryPointer->d_name);
	}	
	::remove(path.c_str());
}

int File::rename(const string &oldPath, const string &newPath) {
    return ::rename(oldPath.c_str(), newPath.c_str());
}


void File::createDir(const string &path) {
	if (!File::isExist(path)) {
		string fullPath(path);	
		/**if path end up with '/' then delete it*/
		if (0 == path.compare(path.length() - 2, 1, "/")) {
			fullPath = path.substr(0, path.length() - 1);
		}
		
		int pos = fullPath.find('/', 1);
		string dirPath;
		while (pos > 0) {
			dirPath = fullPath.substr(0, pos + 1);
			::mkdir(dirPath.c_str(), 0755);
			pos = fullPath.find('/', pos + 1);
		}
		::mkdir(fullPath.c_str(), 0755);
	}
}

const string &File::getPath() const {
	return _path;
}

const string File::getBaseName() const {
	size_t baseNameOff = _path.rfind('/', _path.length() - 1);
	

	return _path.substr(baseNameOff + 1, _path.length() 
						- baseNameOff - 1);

}

string File::getBaseName(const string &path) {
	size_t baseNameOff = path.rfind('/', path.length() - 1);
	
	
	return path.substr(baseNameOff + 1, path.length() 
						- baseNameOff - 1);


}

const string File::getDirName() const {
	if (_path.compare(0, 1, "/") != 0) {
		return string("null");
	}
	size_t dirNameOff = _path.rfind('/', _path.length() - 1);
	return _path.substr(0, dirNameOff + 1);
}

int File::open(const OpenMode mode) {
	const char *fmode = "";
	
	if (_fp != NULL) {
		close();
	}
	
    if (mode & OPEN_MODE_WRITE) {
        if (mode & OPEN_MODE_APPEND) {
            /* write, read, create, append */
            /* (append implies create)     */
            fmode = "a+";
        } else {
            if ((mode & OPEN_MODE_CREATE) || (mode & OPEN_MODE_TRUNCATE)) {
                /* write, read, create, truncate                      */
                /* (truncate implies create, create implies truncate) */
                fmode = "w+";
            } else {
                /* write, read */
                fmode = "r+";
            }
        }
    } else {
        /* read only */
        fmode = "r";
    }
	
	_fp = ::fopen(_path.c_str(), fmode);
	if(NULL == _fp) {
		return LEMON_FAILURE;
	}

	if (_path.compare(0, 1, "/") != 0) {
		char buff[FILE_PATH_MAX_LEN] = {0};
		if (realpath(_path.c_str(), buff) != NULL) {
			_path = buff;//update _path to absolute path
		}
	}

	return LEMON_SUCCESS;
}

void File::close() {
	if (_fp != NULL) {
		::fclose(_fp);
		_fp = NULL;
	}
}

size_t File::read(char *buff, const size_t len) const {
	size_t ret = 0;

	if (NULL == _fp) {
		return ret;
	}
	ret = ::fread(buff, len, 1, _fp);
	
	return len * ret;
}

size_t File::write(const unsigned char *buff, const size_t len) const {
	size_t ret = 0;

	if (NULL == _fp) {
		return ret;
	}
	ret = ::fwrite(buff, len, 1, _fp);
	
	return len * ret;
}

void File::ffulsh() {
	::fflush(_fp);
}
