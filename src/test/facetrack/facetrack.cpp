/* 
 * ===================================================================================== 
 * 
 *       Filename:  facetrack.cpp
 * 
 *    Description:  人脸运动预测
 * 
 *        Version:  1.0 
 *        Created:  2017年04月10日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi
 *                                    
 * ===================================================================================== 
 */ 
#include "facetrack.h"
#define  Maxdiff     (25)

enum  direction
{
  up    =1,
  down  =2,
  left  =3,
  right =4 
}move_direct;
int judgeborder(int x,int y,int slope_x,int slope_y,int width,int high)
{   
   if(x<20||(x+slope_x)<20)    return 3;
   if(abs(x-width)<20||abs(x-width+slope_x)<20) return 4;
   if(y<20||(y+slope_y)<20)    return  1;
   if(abs(y-high)<20||abs(y-high+slope_y)) return 2;
   return 0;

}

static bool facepredict(int x_slope,int y_slope,int framediff,int x,int y,int cols,int rows)//欧式距离判断
{
    
     if(judgeborder(x,y,x_slope,y_slope,rows,cols)==0)
         return false;
     int x_diff=x_slope*framediff+x;
     int y_diff=y_slope*framediff+y;
     int O_distance;
      
    O_distance=sqrt(y_diff*y_diff+x_diff*x_diff);

    if(O_distance<=Maxdiff||sqrt(x*x+y*y)<=Maxdiff)  return true;
    else   return false;


}
void judgeface(int framediff,std::vector<struct face_locate> vec_face_locate,Mat img,std::string filedir,unsigned int& people_id,std::vector<struct _face_info> &vec_face_info,Mat &backtrack,std::string filepath)
{  
   
        Mat  img_face;
        std::vector<struct _face_info>::iterator it =vec_face_info.begin();
        //printf("%x\n",*it->);
    //printf("it->location[0].x:%d<><>it->framenum:%d\n",it->location[0].x,it->framenum);
	here:while(it!=vec_face_info.end())
	   { 
            
		  int temp;
         
		  it->framenum%=(MAXSIZE);
		  if((it->framenum-1)<0)     temp=(MAXSIZE-1);
		   else                      temp=it->framenum-1;
          std::vector<struct face_locate>::iterator item = vec_face_locate.begin();
         
	 for(;item!= vec_face_locate.end();++item)
              {  
                  int x_diff=item->x_slope=item->x-it->location[temp].x;
                  int y_diff=item->y_slope=item->y-it->location[temp].y;
             //printf("x_diff:%d<><>y_diff:%d<><>item->x:%d\n",abs(item->x-it->location[temp].x),abs(item->y-it->location[temp].y),item->x);
             //O_distance=sqrt(y_diff*y_diff+x_diff*x_diff);

                 if(facepredict(it->location[it->framenum-1].x_slope,it->location[it->framenum].y_slope,framediff,x_diff,y_diff,img.cols,img.rows))//欧式距离
		   {    
		       it->location[it->framenum].x=item->x;
		       it->location[it->framenum].y=item->y;
                       it->location[it->framenum].x_slope=(int)(item->x_slope/framediff);
                       it->location[it->framenum].y_slope=(int)(item->y_slope/framediff);
		       it->framenum++;
                       it->count++;
       		       item->people_id=it->people_id;
                       cv::line(backtrack,Point(it->location[temp].x,it->location[temp].y),Point(item->x,item->y),Scalar(255,255,255));
                      // backtrack.at<unsigned char>(item->x,item->y)=51*(it->people_id%6);
                        char temp [40] = {0};
                       std::string temp_filedir=filedir;
                       snprintf(temp, 40, "/people[%d]/",it->people_id);
                       temp_filedir+=temp;          
                     if(!Isexistfile((char *)temp_filedir.c_str(),filepath))
                       { 
                         printf("create filepath failed\n"); 
                         exit(-1);
                       }
                       img_face=img(item->face_rect);
                     char image_name[40]; 
                     sprintf(image_name, "%d%s",it->count,".jpg");//保存的图片名  
                   
		     std::string image = temp_filedir+image_name;  
		     IplImage qt;
		     qt=IplImage(img_face);
		     cvSaveImage(image.c_str(),&qt);   
                       it++;
                      vec_face_locate.erase(item);
                  
                    
                      goto here;
		    }
           
		}
                
              vec_face_info.erase(it);
             

    
            }

     std::vector<struct face_locate>::iterator item = vec_face_locate.begin();
     for(;item!=vec_face_locate.end();++item)
       {       
            struct _face_info vec_data;               
	    
	      vec_data.location[0].x=item->x;
	      vec_data.location[0].y=item->y;
              vec_data.people_id=(++people_id);
              vec_face_info.push_back(vec_data);
             //cv::line(backtrack,Point(it->location[temp].x,it->location[temp].y),Point(item->x,item->y),Scalar(51*(it->people_id%6),0,0));
              char temp [40] = {0};
              std::string temp_filedir=filedir;
              snprintf(temp, 40, "/people[%d]/",people_id);
              temp_filedir+=temp;          
              if(!Isexistfile((char *)temp_filedir.c_str(),filepath))
                 { 
                         printf("create filepath failed\n"); 
                         exit(-1);
                 }
              img_face=img(item->face_rect);
              char image_name[40]; 
              sprintf(image_name, "%d%s",1,".jpg");//保存的图片名  
              
	      std::string image = temp_filedir+image_name;  
	      IplImage qt;
	      qt=IplImage(img_face);
	      cvSaveImage(image.c_str(),&qt);   
       
          }
    
}


