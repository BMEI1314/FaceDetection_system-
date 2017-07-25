/* 
 * ===================================================================================== 
 * 
 *       Filename:  auto_window.cpp 
 * 
 *    Description:  1.已知f,与相机内部参数计算人脸检测最小窗口大小。2.相机标定。
 * 
 *        Version:  1.0 
 *        Created:  2017年04月01日
 *       Revision:  none 
 *       Compiler:  g++
 *       Author:  zhangqi    
 * 
 * ===================================================================================== 
 */ 
#include "header.h"
int dectfps;
int Real2PixelLength(int reallength,int distance,float f,float ccdlenth)//都是mm级的运算
{           
           int window_width;
           int pixl_width;
           int pixl_high;
           pixl_width=((reallength*f/distance)*pixel_width)/ccdlenth;
           pixl_high=((reallength*f/distance)*pixel_high)/ccdlenth; 
           window_width=pixl_width;
           printf("window_width:%d\n",window_width);
	 return window_width;
}

void auto_dectfps_init(void)
{

   dectfps=1;
}

int audo_dectfps()
{ 
          if(loseface_count==1)
	   dectfps=1;
	  if(loseface_count>10)
	   {
	      dectfps++;
	   }
	   if(dectfps>40) dectfps=1;
	  return dectfps;
}

int caliberatcam(char *imgname)
{       
        Mat imageInput=imread(imgname);
        int temp=0;
        Size image_size;  /* 图像的尺寸 */
	Size board_size = Size(4,6);/* 标定板上每行、列的角点数 */
	vector<Point2f> image_points_buf;  /* 缓存每幅图像上检测到的角点 */
	image_size.width = imageInput.cols;
	image_size.height =imageInput.rows;			
		/* 提取角点 */
         if (0 == findChessboardCorners(imageInput,board_size,image_points_buf))
		{			
			cout<<"can not find chessboard corners!\n"; //找不到角点
			exit(1);
	        } 
	else 
		{
			Mat view_gray;
			cvtColor(imageInput,view_gray,CV_RGB2GRAY);
			/* 亚像素精确化 */
			find4QuadCornerSubpix(view_gray,image_points_buf,Size(5,5)); //对粗提取的角点进行精确化
	                for(int i=0;i<board_size.width-1;i++)
                     {
                        temp=(int)(image_points_buf[i+1].y-image_points_buf[i].y);
                     }
		}
         
  return temp;
}

