
/*数组实现循环队列*/

#include<stdio.h>
#include<stdlib.h>

#define SUCCESS 0		//操作成功
#define ERROR	-1		//操作失败
#define MaxSize	50		//数组最大长度

typedef	int	TYPE;		//数组类型

//入队列
//参数说明：open 数组，in 队首指针，out 队尾指针，value 入队的值
int EnQueue(TYPE *open,int *in,int *out,TYPE value){
	int temp;
	temp = ((*in)+1)%MaxSize;		//队满条件 in+1 = out
	if(temp == (*out)){
		return ERROR;
	}
	open[*in] = value;
	(*in) = temp;
	return SUCCESS;
}
//出队列
//参数说明：open 数组，in 队首指针，out 队尾指针
TYPE GetQueue(TYPE *open,int *in,int *out){
	TYPE value;
	if((*in) == (*out)){		//队空条件 in == out
		return ERROR;
	}
	value = open[*out];
	open[*out] = 0;				//出队，置位
	(*out) = ((*out)+1)%MaxSize;
	return value;
}

int main(){
	TYPE open[MaxSize];
	int in,out;		//队首 、队尾
	int i;
	int value;
	value = 0;
	in = 0;
	out = 0;
	//memset(open,0,sizeof(open));
	for(i = 0;i < MaxSize; i++){
		open[i] = 0;
	}
	EnQueue(open,&in,&out,1);
	EnQueue(open,&in,&out,2);
	EnQueue(open,&in,&out,3);
	do{								//全部出队
		value = GetQueue(open,&in,&out);
		if(value != -1){ 
			printf("EnQueue = %d\n",value);
		}
	}while(value != -1);
	printf("********************************\n");
	EnQueue(open,&in,&out,4);
	do{
		value = GetQueue(open,&in,&out);
		if(value != -1){ 
			printf("EnQueue = %d\n",value);
		}
	}while(value != -1);
	printf("********************************\n");
	EnQueue(open,&in,&out,2);
	EnQueue(open,&in,&out,3);

	for(i = 0;i < MaxSize; i++){
		printf("EnQueue[%d] = %d\n",i,open[i]);
	}
	system("pause");
}