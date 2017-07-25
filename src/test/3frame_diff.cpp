/* 
 * ===================================================================================== 
 * 
 *       Filename:  3frame_diff.cpp 
 *       Description:  1.唤醒算法的机制:帧间差分法，大律法与rgb的肤色检测算法. 通过帧间差分法提取运动背景,对   
 *                       运动背景rgb肤色检测.通过比较运动背景中的肤色成分分析,来判断是否有人的闯入.
 *       Version:  1.0 
 *       Created:  2017年04月01日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi    
 * 
 * ===================================================================================== 
 */ 
#include "header.h"
float ratio;
static int threshold_diff1;//设置阈值  
static bool otsuopen=true;


static int Otsu(IplImage* src)  
{  
	int height=src->height;  
	int width=src->width;      
        int thres=0;
	//histogram  
	float histogram[256] = {0};  
	for(int i=0; i < height; i++)
	{  
		unsigned char* p=(unsigned char*)src->imageData + src->widthStep * i;  
		for(int j = 0; j < width; j++) 
		{  
			histogram[*p++]++;  
		}  
	}  
	//normalize histogram & average pixel value 
	int size = height * width;  
	float u =0;
	for(int i = 0; i < 256; i++)
	{  
		histogram[i] = histogram[i] / size;  
		u += i * histogram[i];  //整幅图像的平均灰度
	}  

	   
	float maxVariance=0;  
	float w0 = 0, avgValue  = 0;
        int i;
	for(i= 0; i < 256; i++) 
	{  
		w0 += histogram[i];  //假设当前灰度i为阈值, 0~i 灰度的像素(假设像素值在此范围的像素叫做前景像素) 所占整幅图像的比例
		avgValue  += i * histogram[i]; // avgValue/w0 = u0

		float t = avgValue/w0 - u;  //u0-u
		float variance = t * t * w0 /(1 - w0);  
		if(variance > maxVariance) 
		{  
                        thres=i;
			maxVariance = variance;  
			
		}  
	}  
	return thres;  
} 
const bool threeframediff(Mat img_src1,Mat img_src2)  
{    
 printf("[%s] -- [%d] -- threeframediff enter\r\n", __FUNCTION__, __LINE__);
            int height[img_src1.cols]={0};
            int width[img_src1.rows]={0};
            int heightface=0;
            int widthface=0; 
	    Mat img_dst,gray1,gray2;  
	    Mat gray_diff1,gray_diff2;//存储2次相减的图片  
	    Mat gray_diff11,gray_diff12;   
	    Mat gray;
     
            cvtColor(img_src1,gray1,CV_BGR2GRAY);   
            cvtColor(img_src2,gray2,CV_BGR2GRAY);    
            Sobel(gray1,gray1,  CV_8U,1,0,3,0.4,128);  
            Sobel(gray2,gray2,  CV_8U,1,0,3,0.4,128);  
            subtract(gray2,gray1,gray_diff11);//第二帧减第一帧  
            subtract(gray1,gray2,gray_diff12);  
            add(gray_diff11,gray_diff12,gray_diff1); 
            dilate(gray_diff1,gray_diff1,Mat());//膨胀
            erode(gray_diff1,gray_diff1,Mat()); 
            IplImage temp(gray_diff1);   
            while(otsuopen)
            {
             threshold_diff1=Otsu(&temp);
             otsuopen=false;
            } 
            for(int i=0;i<gray_diff1.rows;i++)  
                for(int j=0;j<gray_diff1.cols;j++)  
                {  
                    if(abs(gray_diff1.at<unsigned char>(i,j))>=threshold_diff1)
                       {
                        //  gray_diff1.at<unsigned char>(i,j)=255;            //第一次相减阈值处理  
                          int b=img_src1.at<Vec3b>(i,j)[0];             //肤色检测
                          int g=img_src1.at<Vec3b>(i,j)[1];
                          int r=img_src1.at<Vec3b>(i,j)[2];
                    
                          if((r>95&&g>40&&b>20&&(r-b)>15&&(r-g)>15)||(r>200&&g>210&&b>170&&(r-b)<=15&&r>b&&g>b))    //肤色检测
                          {
                             
                             height[j]++;
                             width[i]++;
                          } 
                       }
                  if(height[j]>32) heightface++;
                  else            
                        {  
                          if(heightface>32&&widthface>32)
                            { 
                              printf("%d----%d\n",heightface,widthface);
                              return true;  
                            }
                           heightface=0;
                        }
                  if(width[i]>32)  widthface++;
                  else           
                        {  
                          if(heightface>32&&widthface>32)
                             { 
                              printf("%d---%d\n",heightface,widthface);
                              return true;  
                            } 
                           widthface=0;
                        }
                    
                }  
           //medianBlur(gray,gray,3);//中值滤波
            //dilate(gray,gray,Mat());//膨胀
            //erode(gray,gray,Mat());//腐蚀 

       
             return false;
}  
