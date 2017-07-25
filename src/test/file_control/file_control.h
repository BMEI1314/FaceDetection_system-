/* 
 * ===================================================================================== 
 * 
 *       Filename:  file_control.h
 * 
 *    Description:  
 * 
 *        Version:  1.0 
 *        Created:  2017年04月01日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi    
 * 
 * ===================================================================================== 
 */ 
#ifndef FILE_CONTROL_H_
#define FILE_CONTROL_H_
#include <string.h>
#include <iostream>
#include <stdio.h>  
#include <stdlib.h>
#include<sys/types.h>
#include <sys/time.h>
#include<sys/stat.h>
#include<unistd.h>
#include <boost/filesystem.hpp>
#include <boost/bind.hpp>
#include <boost/iostreams/device/mapped_file.hpp>
#include <vector>
using namespace std;
using namespace boost;
extern  unsigned int  countnum;
extern unsigned long framenum;

char *createfiledir(const timeval &tv,const char *filedir,const char *predix);//指针函数
void __del_timeout_file(const std::string& _path);
static bool Isexistfile (const char *filepath,const std::string& filedir);
#endif
