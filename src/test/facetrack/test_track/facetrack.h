#ifndef TRACK_H_
#define TRACK_H_

#include <stdio.h>
#include <iostream>
#include <string>  
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <vector>
#include "file_control.h"
#include"header.h"
using namespace std;
#define  MAXSIZE     (3)
#define  Maxdiff     (50)
namespace facetrack{
typedef struct face_locate
{
  face_locate():x(0),y(0){}
  int x;
  int y; 
  int people_id;
  cv::Rect face_rect;
}face_locate;

typedef struct _face_info
{ 
  _face_info():framenum(1){}
  int  framenum;
  struct  face_locate location[MAXSIZE];
  int people_id;
}_face_info;


   void judgeface(int framediff,std::vector<struct face_locate> vec_face_locate,Mat img,int framenum,std::string filedir,int people_id);
   int judgeborder(int x,int y,int slope_x,int slope_y,int width,int high);
 
   std::vector<struct _face_info> vec_face_info;
    std::vector<struct face_locate> vec_face_locate;
}; /* namespace facetrack*/

#endif

