/* 
 * ===================================================================================== 
 * 
 *       Filename:  facetrack.cpp
 * 
 *    Description:  定义几种状态1.走向镜头方向.2.垂直于镜头方向
 * 
 *        Version:  1.0 
 *        Created:  2017年04月10日
 *       Revision:  none 
 *       Compiler:  g++ 
 *       Author:  zhangqi
 *                                    
 * ===================================================================================== 
 */ 
//#include "facetrack.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string>  
#include <math.h>
#include <unistd.h>
#include <vector>
using namespace std;
#define  MAXSIZE     (3)
#define  Maxdiff     (10)
struct face_locate
{
  face_locate():x(0),y(0),Isstore(false){}
  int x;
  int y; 
  bool  Isstore;
};

struct _face_info
{ 
  _face_info():framenum(1){}
  int  framenum;
  struct  face_locate location[MAXSIZE];
};
 std::vector<struct _face_info> vec_face_info;

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


void judgeface(int framediff,int num_face,std::vector<struct face_locate> vec_face_locate)
{  
        int O_distance=0;
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
		       item->Isstore=true;
                       vec_face_locate.erase(item);
                       it++;
                       printf("isnot\n\n");
                      goto here;
		    }
           
		}
              
              vec_face_info.erase(it);
              // it++;
              printf("dele\n\n\n");
     
            }

    std::vector<struct face_locate>::iterator item = vec_face_locate.begin();
     for(;item!=vec_face_locate.end();++item)
   {  

         
            struct _face_info vec_data;               
	      printf("11111\n");
	      vec_data.location[0].x=item->x;
	      vec_data.location[0].y=item->y;
              vec_face_info.push_back(vec_data);
          }
   // sleep(100); 
    
}
int main()
{
  static std::vector<struct face_locate> vec_face_locate;
  for(int i=1;i<10;i++)
   
  { 
       struct face_locate locate_data;              
       locate_data.x=100*i;
       locate_data.y=200*i;
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
   for(int i=1;i<3;i++)   
  {judgeface(1,9,vec_face_locate);
  //sleep(10);
      std::vector<struct _face_info>::iterator it =vec_face_info.begin();
	for(;it!=vec_face_info.end();++it)
        printf("%d<><><><>%d<><>%d\n",it->location[0].x,it->location[0].y,it->framenum);
}
   
return 0;
}
