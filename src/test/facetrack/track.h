#ifndef TRACK_H_
#define TRACK_H_

#include <stdio.h>
#include <iostream>
#include <string>  
#include <stdlib.h>
#include <math.h>
#include <unistd.h>
#include <vector>
//#include "header.h"
#include "../file_control/file_control.h"
#include"../../../include/header.h"
using namespace std;

namespace facetrack{
typedef struct face_locate
{
  face_locate():x(0),y(0){}
  int x;
  int y; 
  int people_id;
  cv::Rect face_rect;
};

typedef struct _face_info
{ 
  _face_info():framenum(1){}
  int  framenum;
  struct  face_locate location[MAXSIZE];
  int people_id;
};


   bool   judgeface(int framediff);
   int facetrack::judgeborder(int x,int y,int slope_x,int slope_y,int width,int high);
   void facetrack::move_predict();
  typedef std::vector<struct _face_info> vec_face_info;
  typedef  std::vector<struct face_locate> vec_face_locate;
}; /* namespace facetrack*/

#endif

