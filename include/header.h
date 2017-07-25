
/* 
 * ===================================================================================== 
 * 
 *       Filename:  header.h 
 * 
 *    Description:  
 * 
 *        Version:  1.0 
 *        Created:  2017年04月01日
 *       Revision:  none 
 *       Compiler:  gcc 
 *       Author:  zhangqi    
 * 
 * ===================================================================================== 
 */ 
#ifndef HEADER_H_
#define HEADER_H_
#include <opencv2/highgui/highgui.hpp>  
#include <opencv2/imgproc/imgproc.hpp>  
#include <opencv2/core/core.hpp>  
#include <opencv2/opencv.hpp>
#include <string.h>
#include <iostream>
#include <stdio.h>
using namespace cv;  
using namespace std;


extern int ccdlength;//传感器的长度
extern int window_width;
extern int loseface_count;//连续没有检测到人脸
extern int pixel_width;
extern int pixel_high;//分辨率的长,宽
void auto_dectfps_init(void);
const bool threeframediff(Mat img_src1,Mat img_src2);
int Real2PixelLength(int reallength,int distance,float f,float ccdlenth);
int audo_dectfps();
#endif
