/* 
 * ===================================================================================== 
 * 
 *       Filename:  file_control.cpp
 * 
 *    Description:  file manager: 1.make filepath name
 *                                2.make file dir (support multi_dir)
 *                                3.delete timeout file(use boost lib)
 * 
 *        Version:  1.0 
 *        Created:  2017年04月10日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi    
 * 
 * ===================================================================================== 
 */ 
#include "file_control.h"
extern unsigned int peopleID;
static unsigned int  kMaxAliveTime= 24*60*60; //1 day in second
static char filepath[1024]={0};
const int create_multi_dir(char *path)
{  
        int i, len;
        
        len = strlen(path);
      
        char dir_path[len];
        strncpy(dir_path, path, len);
        for (i=0; i<len+1; i++)
        {
                if (dir_path[i] == '/' && i > 1)
                {
                        dir_path[i]='\0';
                        if (access(dir_path, F_OK) < 0)
                        {
                                if (mkdir(dir_path, 0755) < 0)
                                {
                                        printf("mkdir=%s:msg=%s\n", dir_path, strerror(errno));
                                        return -1;
                                }
                        }
                        dir_path[i]='/';
                }
        }

        return 0;
}
static void makefilename(char* _filepath,const char *filedir,const char *predix,const timeval &tv,int len)
{     
    time_t sec = tv.tv_sec;
    struct tm tcur = *localtime((const time_t*)&sec);
    std::string filepath;
    filepath+=filedir;
    filepath += "/";
    filepath += predix;
    char temp [40] = {0};
    snprintf(temp, 40, "_%02d%02d%02d", 1 + tcur.tm_mon, tcur.tm_mday,tcur.tm_hour);
    filepath += temp;
    filepath+="/";
    strncpy(_filepath,filepath.c_str(), len-1);//不像strcpy()，strncpy()不会向dest追加结束标记'\0'
    _filepath[len - 1] = '\0'; //注意要加'\0',否则会创建文件夹错误
    

}
/*static void __del_files(const std::string& _forder_path) //删除文件
{
    
    boost::filesystem::path path(_forder_path);
    if (!boost::filesystem::is_directory(path)) {
        return;
    }
    
    boost::filesystem::directory_iterator end_iter;
    for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
        if (boost::filesystem::is_regular_file(iter->status()))
        {
            boost::filesystem::remove(iter->path());
        }
    }
}*/

void __del_timeout_file(const std::string& _path) //删除超时文件
{
    time_t now_time = time(NULL);//初始化时间
    
    boost::filesystem::path path(_path);
    
    if (boost::filesystem::exists(path) && boost::filesystem::is_directory(path)){
        boost::filesystem::directory_iterator end_iter;
        for (boost::filesystem::directory_iterator iter(path); iter != end_iter; ++iter) {
            time_t fileModifyTime = boost::filesystem::last_write_time(iter->path());
            if (now_time > fileModifyTime && now_time - fileModifyTime > kMaxAliveTime) {
             // if (boost::filesystem::is_regular_file(iter->status())) { //判断是否为目录还是文件
                    boost::filesystem::remove_all(iter->path());//目录为空时，remove出错，所以使用remove_all
             // }
             //   else if (boost::filesystem::is_directory(iter->status())) {
             //     __del_files(iter->path().string());
             //   }
            }
        }
    }
}
/*
void dele_timeoutfile (const timeval &tv)//删除超时文件
{ 
   char tmoutfilepath[1024];
   int temphour=0;
   int tempday=0;
   char hour[2];
   char day[2];
   int len=1024;
  //  strncpy(tmoutfilepath,AliveTime.filename.c_str(),len);
    struct tm tmout=*localtime(&tv.tv_sec);
    for()
    time_t now_time = time(NULL);//初始化时间
    makefilename(tmoutfilepath,filedir,predix,tv,1024);
   if(!remove(tmoutfilepath)) printf("remove timeoutfile%s:",tmoutfilepath);
  
}
*/
bool Isexistfile (char *filepath,const std::string& filedir)
{   
   if(access(filepath,F_OK) == -1) //判断存在与否   
        {
          printf("This file does not exist!\n"); 
          __del_timeout_file(filedir);//每次准备新建文件夹时检查是否有超时文件  
        if(!create_multi_dir(filepath)) //返回0表示成功
           { 
           return true;
          }
          else
           return  false; 
         }
   else
   return true;
}
char *createfiledir(const timeval &tv,const char *filedir,const char *predix)//指针函数
{        
     makefilename(filepath,filedir,predix,tv,1024);
     std::string filedirname;
     filedirname+=filedir;
     if(!Isexistfile(filepath,filedirname)) { printf("create filepath failed\n"); exit(-1);}
     return filepath;  
}
