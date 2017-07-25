/* 
 * ===================================================================================== 
 * 
 *       Filename:  facetrack.cpp
 * 
 *    Description:  人脸运动预测,定义几种状态1.走向镜头方向.2.垂直于镜头方向
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

#define  MAXSIZE     (3)
#define  Maxdiff     (10)

 //std::vector<struct _face_info> vec_face_info;

enum  direction
{
  up    =1,
  down  =2,
  left  =3,
  right =4 
}move_direct;
int facetrack::judgeborder(int x,int y,int slope_x,int slope_y,int width,int high)
{   
   if(x<20||(x+slope_x)<20)    return 3;
   if(abs(x-width)<20||abs(x-width+slope_x)<20) return 4;
   if(y<20||(y+slope_y)<20)    return  1;
   if(abs(y-high)<20||abs(y-high+slope_y)) return 2;
   return 0;

}
void facetrack::move_predict()
{
  
  
}

void facetrack::judgeface(int framediff,int num_face,std::vector<struct face_locate> vec_face_locate,Mat img,int framenum)
{  
        int O_distance=0;
        Mat  img_face
        std::vector<struct _face_info>::iterator it =vec_face_info.begin();
        //printf("%x\n",*it->);
    
	here:while(it!=vec_face_info.end())
	   { 
             printf("it->location[0].x:%d<><>it->framenum:%d\n",it->location[0].x,it->framenum);
              printf("coming in\n");
		  int temp;
         
		  it->framenum%=(MAXSIZE);
		  if((it->framenum-1)<0)     temp=(MAXSIZE-1);
		   else                      temp=it->framenum-1;
            printf("temp:%d\n",temp);

          std::vector<struct face_locate>::iterator item = vec_face_locate.begin();
	 for(;item!= vec_face_locate.end();++item)
              {  
                  int x_diff=abs(item->x-it->location[temp].x);
                  int y_diff=abs(item->y-it->location[temp].y);
             printf("x_diff:%d<><>y_diff:%d<><>item->x:%d\n",abs(item->x-it->location[temp].x),abs(item->y-it->location[temp].y),item->x);
           
             O_distance=sqrt(y_diff*y_diff+x_diff*x_diff);
             printf("O_distance:%d\n",O_distance);
        // sleep(10);
                 if(O_distance<=Maxdiff)
		   {    
		       it->location[it->framenum].x=item->x;
		       it->location[it->framenum].y=item->y;
		       it->framenum++;
       		       item->people_id=it->people_id;
                        char temp [40] = {0};
                       std::string temp_filedir=filedir;
                       snprintf(temp, 40, "/people[%d]/",it->people_id);
                       temp_filedir+=temp;          
                     if(!file_control::Isexistfile(temp_filedir,filedir))
                       { 
                         printf("create filepath failed\n"); 
                         exit(-1);
                       }
                       img_face=img(item->face_rect);
                     char image_name[40]; 
                     sprintf(image_name, "%d%s",framenum,".jpg");//保存的图片名  
                     printf("store image:%s\n",image_name);
		     std::string image = temp_filedir+image_name;  
		     IplImage qt;
		     qt=IplImage(img_face);
		     cvSaveImage(image.c_str(),&qt);   
      
                      vec_face_locate.erase(item);
                  
                      printf("isnot\n\n");
                      goto here;
		    }
           
		}
       
              vec_face_info.erase(it);
              printf("dele\n");
    
            }

    std::vector<struct face_locate>::iterator item = vec_face_locate.begin();
     for(;item!=vec_face_locate.end();++item)
       {       
            struct _face_info vec_data;               
	      printf("11111\n");
	      vec_data.location[0].x=item->x;
	      vec_data.location[0].y=item->y;
              vec_data.people_id=(++people_id);
              vec_face_info.push_back(vec_data);
              char temp [40] = {0};
              std::string temp_filedir=filedir;
              snprintf(temp, 40, "/people[%d]/",people_id);
              temp_filedir+=temp;          
              if(!file_control::Isexistfile(temp_filedir,filedir))
                 { 
                         printf("create filepath failed\n"); 
                         exit(-1);
                 }
              img_face=img(item->face_rect);
              char image_name[40]; 
              sprintf(image_name, "%d%s",framenum,".jpg");//保存的图片名  
              printf("store image:%s\n",image_name);
	      std::string image = temp_filedir+image_name;  
	      IplImage qt;
	      qt=IplImage(img_face);
	      cvSaveImage(image.c_str(),&qt);   
       
          }
    
}
/*
int main()
{
  //static std::vector<struct face_locate> vec_face_locate;
  for(int i=1;i<10;i++)
   
  { 
       struct face_locate locate_data;              
       locate_data.x=10*i;
       locate_data.y=20*i;
       vec_face_locate.push_back(locate_data);  
     //locate[i].x=10*i;locate[i].y=20*i;//测试这个结构体
   }
  for(int i=1;i<5;i++)
   {        
              struct _face_info vec_data;               
	      vec_data.location[0].x=15*i;
	      vec_data.location[0].y=25*i;          
              vec_face_info.push_back(vec_data);     
  }
   for(int i=1;i<10;i++)   
  {judgeface(1,9,vec_face_locate);
  //sleep(10);
      std::vector<struct _face_info>::iterator it =vec_face_info.begin();
	for(;it!=vec_face_info.end();++it)
        printf("%d<><><><>%d<><>%d\n",it->location[0].x,it->location[0].y,it->framenum);
}
   
return 0;
}*/
