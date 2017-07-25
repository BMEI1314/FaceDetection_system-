#include <stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>


#define MAXSIZE 10

typedef int Status;
//使用循环队列
typedef struct face_center
{
  int x;
  int y;
  int width;
}QElemType;

typedef struct {

    QElemType Data[MAXSIZE]; 
    int front;
    int rear;
}XHQueue;

int getRand(int ,int );//当时测试随机数的，两个变量是上下限
void InitQueue(XHQueue *);  //初始化队列
Status EnQueue(XHQueue *,QElemType ); //入队
Status DeQueue(XHQueue *,QElemType *);//出队
Status IsEmpty(XHQueue *); //判断是否为空

//队列初始化
void InitQueue(XHQueue *Q){
    Q->front=0;
    Q->rear=0;


}

//入队

Status EnQueue(XHQueue *Q,QElemType E){

    if((Q->rear+1)%MAXSIZE ==Q->front)//队列已满
    {
        return 0;
    }
    Q->Data[Q->rear]=E;
    Q->rear=(Q->rear+1)%MAXSIZE;
    return 1;
}

//出队
Status DeQueue(XHQueue *Q,QElemType *E){ 
    if(Q->front==Q->rear){//队列已空
        return 0;
    }
    *E=Q->Data[Q->front];
    Q->front=(Q->front+1)%MAXSIZE;

    return 1;

}

//判断是否为空
Status IsEmpty(XHQueue *Q){
    if(Q->front==Q->rear){
        return 1;
    }
    return 0;

}
/*****函数区***/

bool track(int x,int y)
{
  
  
     QElemTypeRe;
    XHQueue *Q;
    Q=(XHQueue *)malloc(sizeof(XHQueue));
     InitQueue(Q);  
     EnQueue(Q,2);
     EnQueue(Q,0);
     EnQueue(Q,1);
     EnQueue(Q,5);
     while(!IsEmpty(Q)){
         DeQueue(Q,&Re);
         printf("%d\n",Re);
     }
     
    //system("pause");
    return 0;
}
