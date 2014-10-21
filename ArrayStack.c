#include<stdio.h>
#include<stdlib.h>

#define MaxSize	50
#define SUCCESS 0
#define ERROR	-1

int open[MaxSize];

int Push(int *open,int *in,int value);
int  Pop(int *open,int *out);
int main(){
	int in;
	int i;
	in = 0;
	memset(open,0,sizeof(open));
	Push(open,&in,1);
	printf("in = %d\n",in);
	Push(open,&in,2);
	Push(open,&in,3);
	for(i = 0;i < 3; i++){
		printf("value of %d is %d\n",in,Pop(open,&in));
	}
	Push(open,&in,4);
	Push(open,&in,5);
	Push(open,&in,6);
	for(i = 0;i < 3; i++){
		printf("value of %d is %d\n",in,Pop(open,&in));
	}
	for(i = 0;i < 3; i++){
		printf("open[%d] is %d\n",i,open[i]);
	}
}

int Push(int *open,int *in,int value){
	if((*in) > MaxSize)
		return ERROR;
	open[*in] = value;
	(*in)++;
	return SUCCESS;
}
int Pop(int *open,int *out){
	int value;
	if((*out) < 0){
		return ERROR;
	}
	(*out)--;
	value = open[*out];
	return value;
}
int enQueue(int *open,int *in,int value){
	open[*in] = value;

}
