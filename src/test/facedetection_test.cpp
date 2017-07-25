/* 
 * ===================================================================================== 
 * 
 *       Filename:  facedetection.c 
 * 
 *    Description:  对视频de处理
 * 
 *        Version:  1.0 
 *        Created:  2017年04月01日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi    
 * 
 * ===================================================================================== 
 */ 
#include <cstdint>
#include <fstream>
#include <time.h>
#include "header.h"
#include "file_control.h"
#include "face_detection.h"
#include "./facetrack/facetrack.h"
#define         DISTANCE             (3000)
#define         FACELENGTH           (300)
#define          F                    (4)
#define         CMOSLENGTH            (8)
#define          dbg               printf
int mmmmmm=0;
int loseface_count=1;//连续没有检测到人脸
unsigned long framenum=0;//总的帧数
//unsigned int countnum=0;
unsigned int peopleID=0;

int pixel_width;
int pixel_high;//分辨率的长,宽

static char image_name[40],binfile[40]; 
static std::string filedir="./data/test";
static std::string predix="test";

static std::string path;
static bool  dectfaceisopen;
seeta::FaceDetection detector("./model/seeta_fd_frontal_v1.0.bin");

struct timeval tv;

static std::vector<struct _face_info> vec_face_info;
/*
struct th_data
{
   int framediff;
   
   cv::Mat  img;
   std::string path;
   unsigned int peopleID;
   std::vector<struct _face_info> vec_face_info;
   cv::Mat  backtrack;
   std::string filedir;
   std::vector<struct face_locate> face_data_vec;
};
 //judgeface(framediff,face_data_vec,img,path,peopleID,vec_face_info,backtrack,filedir);//track peopleface
        
void *th_judgeface(void *data)
{  
  dbg("[%s] -- [%d] -- th_judgeface enter\r\n", __FUNCTION__, __LINE__);
  printf("%d:::\n",data);
  struct th_data *tmp = (struct th_data *)data;
  cout<<tmp->filedir<<endl;
  judgeface(tmp->framediff,tmp->face_data_vec,tmp->img,tmp->path,tmp->peopleID,tmp->vec_face_info,tmp->backtrack,tmp->filedir);//track peopleface

} */
static void dectface(Mat img,Mat backtrack,int framediff)
{         

          dbg("[%s] -- [%d] --dectface enter\r\n", __FUNCTION__, __LINE__);
	  cv::Mat img_gray;
	  if (img.channels()!= 1)
		 cv::cvtColor(img, img_gray, cv::COLOR_BGR2GRAY);
	   else
		 img_gray = img;
                  
	  seeta::ImageData img_data;
	  img_data.data = img_gray.data;
	  img_data.width = img_gray.cols;
	  img_data.height = img_gray.rows;
	  img_data.num_channels = 1; 
	  std::vector<seeta::FaceInfo> faces = detector.Detect(img_data);
	  cv::Rect face_rect;
	  int32_t num_face = static_cast<int32_t>(faces.size());
	  if(!num_face)      loseface_count++;
	  
         
         if(num_face>0)
      { 
         sprintf(binfile, "%s%s%d%s","./bin","img",mmmmmm,".bin");
                  mmmmmm++;
                 FileStorage fs(binfile, FileStorage::WRITE);
                  fs<<"zhangqi"<<img_gray;
                  fs.release();
	 gettimeofday(&tv, NULL);
        struct face_locate face_data;
        std::vector<struct face_locate> face_data_vec; 
         for(int32_t i=0;i<num_face;i++)
          {
            face_data.x=faces[i].bbox.x+faces[i].bbox.width/2;
            face_data.y=faces[i].bbox.y+faces[i].bbox.height/2;
            face_data.face_rect.x = abs(faces[i].bbox.x);
            face_data.face_rect.y = abs(faces[i].bbox.y);
            face_data.face_rect.width = abs(faces[i].bbox.width);
            face_data.face_rect.height = abs(faces[i].bbox.height);
            //face_data.face_rect=faces[i].bbox;
            face_data_vec.push_back(face_data);
          }
         path=createfiledir(tv,filedir.c_str(),predix.c_str());
        /* struct th_data  *data=(struct th_data *)malloc(sizeof(struct ));
         pthread_t   judgeface_t;
   
         data.framediff=framediff;
 
         data.face_data_vec=face_data_vec;
 
         data.img = img;
      
         data.path=path;
         data.peopleID=peopleID;
  
         data.vec_face_info=vec_face_info;
         data.backtrack=backtrack;
         data.filedir=filedir;
      
         cout<<data.filedir<<endl;
   
          if(pthread_create(&judgeface_t,NULL,th_judgeface,data))
           {      
            dbg("[%s] -- [%d] -- pthread_create failed\r\n", __FUNCTION__, __LINE__);   
           }*/
         judgeface(framediff,face_data_vec,img,path,peopleID,vec_face_info,backtrack,filedir);//track peopleface
      
        loseface_count=1;
        }
         
	
 dbg("[%s] -- [%d] --dectface out\r\n", __FUNCTION__, __LINE__);
}
int main(int argc, char** argv) {

	#ifdef USE_OPENMP
	  cout << "OpenMP is used." << endl;
	#else
	  cout << "OpenMP is not used." << endl;
	#endif
	#ifdef USE_SSE
	  cout << "SSE is used." << endl;
	#else
	  cout << "SSE is not used." <<endl;
	#endif

	  int changefps;
	  dectfaceisopen=true;
         
         dbg("[%s] -- [%d] -- main enter\r\n", __FUNCTION__, __LINE__);
	 // const std::string  videofilepath="rtmp://live.hkstv.hk.lxdns.com/live/hks";//读取rtmp流媒体，rtsp等格式都支持
	 //const std::string videofilepath="rtsp://218.204.223.237:554/live/1/0547424F573B085C/gsfp90ef4k0a6iap.sdp";
	 const std::string  videofilepath="./peopleTest.m4v";
	  VideoCapture capture(videofilepath);
	  if(!capture.isOpened())
		cout<<"fail to open!"<<endl;  
	 Mat frame;
	 pixel_high= capture.get(CV_CAP_PROP_FRAME_HEIGHT);    
	 pixel_width  = capture.get(CV_CAP_PROP_FRAME_WIDTH);
         cv::Mat backtrack(pixel_width,pixel_high,CV_8UC3,Scalar(0));
	 cout<<"pixel_high" <<pixel_high<<"___"<<"pixel_width"<<pixel_width<<endl;  
	 int temp=Real2PixelLength(FACELENGTH,DISTANCE,F,CMOSLENGTH);
	 cout<<"temp:"<<temp<<endl;
//sleep(100);
	 detector.SetMinFaceSize(temp);
	 detector.SetScoreThresh(2.f);//设置分数阈值
	 detector.SetImagePyramidScaleFactor(1.2f);//采样步长
	 detector.SetWindowStep(4, 4); 
	 int readRatio=1;//读取的周期
	 auto_dectfps_init();
         
	   while(1)
	 {    
	      if(!capture.read(frame))  

		{  
		    cout<<"读取视频失败"<<endl;  
		    break;    
		}  
	   if(--readRatio) continue;
	   if(dectfaceisopen){
            //if(0){          
                    long t2 = cv::getTickCount();
		    dectface(frame,backtrack,loseface_count);  
                     long t3 = cv::getTickCount();
                     double secs = (t3 - t2)/cv::getTickFrequency();
                     cout << "dectface takes"<<secs<<"seconds"<< endl;
		    changefps=audo_dectfps();
		    if(changefps<40) 
		           readRatio=changefps;
		    else  {
			   dectfaceisopen=false;
		           readRatio=1;
			   }
	      }
	    else
	   {   
		 Mat frame1;
                     
		 capture>>frame1;
 
                
		 long t0 = cv::getTickCount();
		 dectfaceisopen=threeframediff(frame,frame1);
		 long t1 = cv::getTickCount();
		 double secs = (t1 - t0)/cv::getTickFrequency();
		
		 cout << "threeframediff takes"<<secs<<"seconds"<< endl;
		 readRatio=1;
	    }
	}
             sprintf(image_name, "%s%s","backtrack",".jpg");//保存的图片名  
             printf("haha\n");
             std::string image = filedir+image_name; 
             printf("haha\n");
	     IplImage qt;
		      qt=IplImage(backtrack);
		      cvSaveImage(image.c_str(),&qt); 
          dbg("[%s] -- [%d] --main out\r\n", __FUNCTION__, __LINE__);
	  capture.release(); 
	  cv::destroyAllWindows();
}
