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

struct face_locate
{
  face_locate():x(0),y(0),x_slope(0),y_slope(0){}
  int x;
  int y; 
  int people_id;
  int x_slope;
  int y_slope;
  cv::Rect face_rect;
};
struct _face_info
{ 
  _face_info():framenum(1),count(1){}
  int  framenum;
  struct  face_locate location[MAXSIZE];
  int people_id;
 int count;
};


void judgeface(
	int framediff,
	std::vector<struct face_locate> vec_face_locate,
	Mat img,
	std::string filedir,
	unsigned int &people_id,
	std::vector<struct _face_info> &vec_face_info,
        Mat &backtrack,
        std::string  filepath
	);
int judgeborder(int x,int y,int slope_x,int slope_y,int width,int high);
 
  
#endif

