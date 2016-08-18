/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      FileTest.cpp
 * \author    Damon He   
 * \version   1.0
 * \date      2016/7/12
 * \note      Our operations on files ignore the permission.
 */

#include <iostream>
#include "File.hh"

using namespace lemon;
using namespace std;

const char *TEST_DIR      = "lemon";
const char *TEST_FILE     = "fremaks.txt";
const char *TEST_DIR_NEW  = "hefeng";
const char *TEST_FILE_NEW = "lorent.txt";

void FileRemoveTest() {
	string testFileStr(string(TEST_DIR_NEW) + string("/") 
					   + string(TEST_FILE_NEW));
	cout<<"remove "<<testFileStr<<endl;
	File::remove(testFileStr);
	cout<<"remove "<<TEST_DIR_NEW<<endl;
	File::remove(TEST_DIR_NEW);
}

void FileStaticTest() {
	if (!File::isExist(TEST_DIR)) {
		cout<<TEST_DIR<<" is not exist, create dir"<<endl;
		File::createDir(TEST_DIR);
	} else {
		cout<<TEST_DIR<<" size="<<File::getSize(TEST_DIR)<<" type="
			<<File::getType(TEST_DIR)<<endl;
		string testFileStr(string(TEST_DIR) + string("/") 
						   + string(TEST_FILE));
		string testFileStrNew(string(TEST_DIR) + string("/") 
				              + string(TEST_FILE_NEW));
		if (File::isExist(testFileStr)) {
			cout<<testFileStr<<" size="<<File::getSize(testFileStr)
				<<" type="<<File::getType(testFileStr)<<endl;
			cout<<"rename"<<testFileStr<<" to "<<testFileStrNew
				<<" ret="<<File::rename(testFileStr, testFileStrNew)<<endl;
		} else {
			cout<<testFileStr<<" is not exist"<<endl;
		}
		cout<<"rename"<<TEST_DIR<<" to "<<TEST_DIR_NEW
			<<" ret="<<File::rename(TEST_DIR, TEST_DIR_NEW)<<endl;
		//FileRemoveTest();
	}
}

void FileTest() {
	File file("./fremaks.txt");
	cout<<"file path "<<file.getPath()<<endl;
	cout<<"file base name "<<file.getBaseName()<<endl;
	cout<<"file dir name "<<file.getDirName()<<endl;
	
	cout<<"file open read ret="<<file.open(OPEN_MODE_WRITE | OPEN_MODE_CREATE)<<endl;
	cout<<"file path "<<file.getPath()<<endl;
	cout<<"file base name "<<file.getBaseName()<<endl;
	cout<<"file dir name "<<file.getDirName()<<endl;

	char buff[256] = {0};
	size_t len = 0;	
	//len = file.write("hellofremaks", 12);
	cout<<"write len="<<len<<endl;

	file.close();
	file.open(OPEN_MODE_READ | OPEN_MODE_CREATE);
	len = file.read(buff, 12);
	cout<<"read len="<<len<<"  buff="<<buff<<endl;	
}

int main(int agrc, char *agrv[]) {
	//FileStaticTest();
	FileTest();
}

