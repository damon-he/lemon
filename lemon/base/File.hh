/********************************************************************************
 * Copyright (C) 2016 Damon He. All rights reserved.
 *
 * Use of this source code is governed by a BSD-style license that can be
 * found in the LICENSE file.
 ********************************************************************************/
/**
 * \file      File.hh
 * \author    Damon He  
 * \version   1.0
 * \date      2016/7/12
 * \note      Our operations on files ignore the permission.
 */

#ifndef LEMON_FILE_HH
#define LEMON_FILE_HH

#include <string>
#include "LemonCommon.hh"

using namespace std;

namespace lemon {
	
const unsigned int OPEN_MODE_READ       = 0x01;
const unsigned int OPEN_MODE_WRITE      = 0x02;
const unsigned int OPEN_MODE_CREATE     = 0x04;
const unsigned int OPEN_MODE_TRUNCATE   = 0x08;
const unsigned int OPEN_MODE_APPEND     = 0x10;

const size_t FILE_PATH_MAX_LEN          = 256;

typedef enum {
    FILE_TYPE_NONE,
    FILE_TYPE_REGULAR,
    FILE_TYPE_DIRECTORY,
    FILE_TYPE_SPECIAL,
    FILE_TYPE_OTHER
} FileType;

typedef int OpenMode;

class File {
public:
	/** construction function */
	explicit File(const string &path);
	
	/** deconstruction function */
	~File();
	
	/**
	 * \brief To see if a file exists.
	 * \param[in] path File path.    
	 * \return True if successful, otherwise false.
	 * \note If the path end up with '\' then it return true only it's a 
	 *       directory file.
	 */
    static bool isExist(const string &path);
	
    /**
	 * \brief Get the file type.
	 * \param[in] path File path.    
	 * \return The file type such as FILE_TYPE_REGULAR, FILE_TYPE_DIRECTORY etc.
	 */
	static FileType getType(const string &path);
	
   /**
	 * \brief Get the file size.
	 * \param[in] path File path.    
	 * \return The file size, if it's a directory file then return 
	 *         number of files in this directory.
	 */
	static size_t getSize(const string &path);
   
   /**
	 * \brief Remove the file.
	 * \param[in] path File path.          
	 */
    static void remove(const string &path);
   
    /**
	 * \brief Remove the directory file.
	 * \param[in] path Directory File path.          
	 */
    static void removeDir(const string &path);
	
   /**
	 * \brief Rname the file.
	 * \param[in] old_path File old path.  
	 * \param[in] new_path File new path.  
	 * \return 
	 * On success LEMON_SUCCESS is returned, otherwise LEMON_FAILURE is returned.
	 */   
    static int rename(const string &oldPath, const string &newPath);
   
   /**
	 * \brief Create directory.
	 * \param[in] path Directory file path.  
	 */ 
    static void createDir(const string &path);
   
   /**
	 * \brief Get the path of the file.
	 * \return The path of the file
	 */ 
	const string &getPath() const;
   
   /**
	 * \brief Get the basename of the file.
	 * \return The basename of the file
	 */ 	
	const string getBaseName() const;

    static string getBaseName(const string &path);
   /**
	 * \brief Get the dirname of the file.
	 * \return The dirname of the file
	 */ 	
    const string getDirName() const;
	
   /**
	 * \brief Open the file.
	 * \param[in] mode Decide what mode to open this file.  
	 * \return 
	 * On success LEMON_SUCCESS is returned, otherwise LEMON_FAILURE is returned.
	 */ 
    int open(const OpenMode mode);
   
   /**
	 * \brief Close the file.
	 */    
	void close();
   
    /**
	 * \brief Read from the file.
	 * \param[in] buff Store the data readed from the file. 
	 * \param[in] len The length to read.
	 * \return The real size readed from the file.
	 */ 
	size_t read(char *buff, const size_t len) const;
	
   /**
	 * \brief Write to the file.
	 * \param[in] buff Store the data to  write to the file. 
	 * \param[in] len The length to write.
	 * \return The real size writed to the file.
	 */ 
    size_t write(const unsigned char *buff, const size_t len) const;

	void ffulsh();
	
private:
	DISALLOW_COPY_AND_ASSIGN(File);
	string _path;	
	FILE *_fp;
};


}

#endif