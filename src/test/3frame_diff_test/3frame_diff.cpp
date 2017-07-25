/* 
 * ===================================================================================== 
 * 
 *       Filename:  3frame_diff.cpp 
 *       Description:  待完成：1.唤醒算法的机制不稳定
 *       Version:  1.0 
 *       Created:  2017年04月01日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi    
 *                           
 * ===================================================================================== 
 */                                                                                       
//#include "header.h"
#include <stdio.h>
#include <iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/contrib/contrib.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <vector>
#include <unistd.h>

using namespace std;
using namespace cv;
std::string path="./img";
float ratio;
static int  threshold_diff1; //设置阈值  

#define threshold    (0.1)
unsigned int changepoint=0,facepoint=0;
static bool  changeadd;
int countnum=0;
char image_name[22]; 
int  height[720]={0};
int  width[1280]={0};
static int weightface=0;
static int heightface=0;
static int otsunum=1;
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
        printf("threshold:%d\n",thres);
	return thres;  
} 
const bool threeframediff(Mat img_src1,Mat img_src2)  
{  
    Mat img_dst,gray1,gray2;  
    Mat gray_diff1;//存储2次相减的图片  
    Mat gray_diff11,gray_diff12;  
    Mat gray;
            changepoint=0;
            facepoint=0;
            cvtColor(img_src1,gray1,CV_BGR2GRAY);   
            cvtColor(img_src2,gray2,CV_BGR2GRAY);  
    
            Sobel(gray1,gray1,  CV_8U,1,0,3,0.4,128);  
            Sobel(gray2,gray2,  CV_8U,1,0,3,0.4,128);  
           //sleep(100); 
            subtract(gray2,gray1,gray_diff11);//第二帧减第一帧  
            subtract(gray1,gray2,gray_diff12);  
            add(gray_diff11,gray_diff12,gray_diff1); 
           IplImage temp(gray_diff11);   
            while(otsunum--)
            {
             threshold_diff1=Otsu(&temp);
            }
            dilate(gray_diff1,gray_diff1,Mat());//膨胀
            erode(gray_diff1,gray_diff1,Mat());  
            for(int i=0;i<gray_diff1.rows;i++)  
                for(int j=0;j<gray_diff1.cols;j++)  
                {  
                    if(abs(gray_diff1.at<unsigned char>(i,j))>=threshold_diff1)
                       {
                                    
                          int b=img_src1.at<Vec3b>(i,j)[0];           
                          int g=img_src1.at<Vec3b>(i,j)[1];
                          int r=img_src1.at<Vec3b>(i,j)[2];
                          changeadd=1;
                          changepoint++;
                          if((r>95&&g>40&&b>20&&(r-b)>15&&(r-g)>15)||(r>200&&g>210&&b>170&&(r-b)<=15&&r>b&&g>b))   //肤色检测
                          {  
                              gray_diff1.at<unsigned char>(i,j)=255; 
                             height[i]++;
                             width[j]++;
                             facepoint++;
                           } 
                          else
                          {
                          img_src1.at<Vec3b>(i,j)[0]=0;             //肤色检测
                          img_src1.at<Vec3b>(i,j)[1]=0;
                          img_src1.at<Vec3b>(i,j)[2]=0;
                            }
                       }   
                    else { 
                          img_src1.at<Vec3b>(i,j)[0]=0;             //肤色检测
                          img_src1.at<Vec3b>(i,j)[1]=0;
                          img_src1.at<Vec3b>(i,j)[2]=0;
                         gray_diff1.at<unsigned char>(i,j)=0;  
                         changeadd=0;
                          }    
                    if(height[i]>40) heightface++;
                    else      heightface=0;
                    if(width[j]>40) weightface++;
                    else      weightface=0;
                  // printf("heightface:%d\n",heightface);
                 
                   if(heightface>64)  {printf("heightfaceOK\n");heightface=0;printf("heightface%d\n",heightface);}
                   if(weightface>40)  {printf("weightfaceOK\n");weightface=0;}
          }  
        sprintf(image_name, "%d%s",countnum,".jpg");//保存的图片名  
		      std::string image = path+image_name;  
		      IplImage qt;
		      qt=IplImage(gray_diff1);
		      cvSaveImage(image.c_str(),&qt);
         printf("countnum:[%d]changepoint:[%d]--facepoint[%d]\n",countnum,changepoint,facepoint);
        ratio=(changepoint*1.0)/(1280*720);
        cout<<"ratio:"<<ratio<<endl;
         if(ratio>=threshold) 
             return true;
         else
             return false;

}  

int main()
{         
          Mat frame;
          const std::string  videofilepath="./Test.mov";
	  VideoCapture capture(videofilepath);
	  if(!capture.isOpened())  
		cout<<"fail to open!"<<endl;  
          //while()
	 //{    
     for(int i=0;i<350;i++)
	      if(!capture.read(frame))  

		{  
		    cout<<"读取视频失败"<<endl;  
		    return -1;    
		}  
         Mat frame1;
		 capture>>frame1;
		 long t0 = cv::getTickCount();
		   threeframediff(frame,frame1);
		 long t1 = cv::getTickCount();
		 double secs = (t1 - t0)/cv::getTickFrequency();
	       cout << "threeframediff takes"<<secs<<"seconds"<< endl;
               countnum++;
            for(int i=0;i<1280;i++)
              printf("%d\n",width[i]);
        //}
      capture.release(); 
      cv::destroyAllWindows();
}
