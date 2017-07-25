#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>

struct math
{
  int a;
  int b;
};
int main ()
{
  struct math *matha=(struct math *)malloc(sizeof(struct math)*10);
  //matha->a=10;
  (matha+1)->a=1;
  (matha+2)->a=2;
  (matha+3)->a=3;
  (matha+4)->a=4;
  //(++matha)->b=20;
  *(matha+1)=*(matha+2);
  printf("%d\n",(matha+1)->a);
 return 0;
} 
