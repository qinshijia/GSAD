#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include"GUI.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>


#define MaxVertexNum 	50        	//定义最大顶点数
#define SUCCESS		0		//函数执行成功
#define ERROR		-1		//函数执行失败
//#define TRUE 		1		//已经遍历过
//#define FALSE 		0		//未遍历
//#define DEEPLIMIT 	2		//有界深度优先搜索算法的 深度界限

extern	GtkWidget 	*mainwindow;
extern 	GtkWidget 	*Tstart,*Tend; 
extern	GtkWidget 	*Ffile;
extern	GtkWidget  	*TOpen,*Tclose;
extern	GtkTextBuffer	*bufOpen,*bufclose;
typedef EdgeNode*  TYPE;

int DEEPLIMIT = 2;
EdgeNode*  Open[MaxVertexNum]; 			//open表
EdgeNode*  Close[MaxVertexNum];			//close表
Result	   searchResult;
ALGraph *G;

int Visit[MaxVertexNum] = {FALSE};

//显示邻接点表 
void showALGraph(){
	int i,n;
	EdgeNode  *temp;
	n = G->n;
	for(i = 0;i < n; i++){
		printf("%c的邻接点有：\t",G->adjlist[i].vertex);
		temp = G->adjlist[i].firstedge;
		while(temp != NULL){
			printf("%c\t",G->adjlist[temp->adjvex].vertex);
			temp = temp->next;
		}
		printf("\n");
	}
}


/*建立图的邻接表*/           
int CreatALGraph(const char fileName[])
{
	int i,j,k,cost,evaluate;
	char a;
	EdgeNode *s;       				 //定义边表结点
	FILE *fp;
	if((fp = fopen(fileName,"r")) == NULL){
		perror("Open file error\n");
		showError("文件不存在！");
		return ERROR;
	}
	while(fgetc(fp) != '\n');			//去掉注释行
	fscanf(fp,"%d%d",&i,&j);			 //读入顶点数和边数
	G->n=i;G->e=j;
	fgetc(fp);

	while(fgetc(fp) != '\n');			//去掉注释行
	for(i=0;i<G->n;i++)   				 //建立边表
	{
		fscanf(fp,"%c",&a);
		fgetc(fp);
		G->adjlist[i].vertex=a;    		//读入顶点信息
		G->adjlist[i].firstedge=NULL; 		//边表置为空表
	}

	while(fgetc(fp) != '\n');			//去掉注释行
	for(k=0;k<G->e;k++) 				//建立邻接表
	{ 
		fscanf(fp,"%d%d%d%d",&i,&j,&cost,&evaluate);
		fgetc(fp);
		s=(EdgeNode *)malloc(sizeof(EdgeNode));   	//生成边表结点
		s->adjvex=j;                 			//邻接点序号为j
		s->cost = cost;
		s->evaluate = evaluate;
		s->next=G->adjlist[i].firstedge;
		G->adjlist[i].firstedge=s;     			//将新结点*S插入顶点Vi的边表头部
		s=(EdgeNode *)malloc(sizeof(EdgeNode)); 
		s->adjvex=i;                  			//邻接点序号为i
		s->cost = cost;
		s->evaluate = evaluate;
		s->next=G->adjlist[j].firstedge;   
		G->adjlist[j].firstedge=s;     	 		//将新结点*S插入顶点Vj的边表头部
	}

	for(k=0;k<G->n;k++){					//自连接
		s=(EdgeNode *)malloc(sizeof(EdgeNode));		
		s->adjvex=k;                 			
		s->next=G->adjlist[k].firstedge;
		G->adjlist[k].firstedge=s;     		
	}
	fclose(fp);
	showALGraph();			//打印邻接点表
	return SUCCESS;
}

//查询元素在边表的值
int valueIndex(char value){
	int index;
	for(index = 0;index < (G->n); index++){
		if(G->adjlist[index].vertex == value){
			return index;
		}
	}
	return ERROR;
}

//重置Visit[]
void resetVisit(void){
	int i;
	for(i = 0;i < MaxVertexNum; i++){
		Visit[i] = 0;
	}
}

//释放邻接点表
void freeGraph(){
	int n;	
	EdgeNode *tempNode,*freeNode;
	for(n = 0;n < G->n;n++){
		tempNode = G->adjlist[n].firstedge;
			while(tempNode->next != NULL){
				freeNode = tempNode;
				tempNode = tempNode->next;
				free(freeNode);	
			}
		free(tempNode);
	}
	free(G);


}

/*************************************************************************
   #将元素插入表头
   #参数说明:
   #	*Open  	要插入的表	in/out;
   #	*in 	插入位置	in/out;
   #	value 	插入的值	in;
   #	showFlag显示标志	in; = 0 不显示;= 1 显示Open表;= 2显示close表;
   #返回值:
   #	SUCCESS	插入成功
   #	ERROR	插入失败  
  ***********************************************************************/
int Push(TYPE *Open,int *in,TYPE value,int showFlag){
	if((*in) > MaxVertexNum){		//表满
		printf("stack is fulled!\n");
		return ERROR;
	}
	Open[*in] = value;
	(*in)++;
	switch(showFlag){
		case 1:	showOpenaStack(Open,(*in));break;
		case 2:	showClose(Open, (*in));break;

	}
	return SUCCESS;
}
/*************************************************************************
  #取出表头元素
  #参数说明:
  #	*Open  	元素所在的表	in/out;
  #	*out 	取出元素位置	in/out;
  #	*value 	返回元素的值	out;
  #	showFlag显示标志	in; = 0 不显示;= 1 显示Open表;= 2显示close表;
  #返回值:
  #	SUCCESS	取出成功
  #	ERROR	取出失败  
  ***********************************************************************/
int Pop(TYPE *Open,int *out,TYPE *value,int showFlag){
	if((*out) <= 0){
		return ERROR;
	}
	(*out)--;
	*value = Open[*out];
	switch(showFlag){
		case 1:	showOpenaStack(Open,(*out));break;
		case 2:	showClose(Open,(*out));break;
	}
	return SUCCESS;
}
/*************************************************************************
  #将元素插入队尾
  #参数说明:
  #	*Open  	要插入的队列	in/out;
  #	*in 	队尾位置	in/out;
  #	*out 	队首位置	in/out;
  #	value 	插入的值	in;
  #	showFlag显示标志	in; = 0 不显示;= 1 显示Open表;= 2显示close表;
  #返回值:
  #	SUCCESS	插入成功
  #	ERROR	插入失败  
  ***********************************************************************/
int EnQueue(TYPE *Open,int *in,int *out,TYPE value,int showFlag){
	int temp;
	temp = ((*in)+1)%MaxVertexNum;		//队满条件 in+1 = out
	if(temp == (*out)){
		printf("queue is fulled!\n");
		return ERROR;
	}
	Open[*in] = value;
	(*in) = temp;
	if(showFlag == 1){
		showOpenaQueue(Open,(*in),(*out));
	}
	return SUCCESS;
}
/*************************************************************************
  #将元素插入队尾
  #参数说明:
  #	*Open  	要插入的队列	in/out;
  #	*in 	队尾位置	in/out;
  #	*out 	队首位置	in/out;
  #	value 	插入的值	in;
  # 	mode	排序模式	in; = 0 按cost排序; = 1 按evaluate排序
  #	showFlag显示标志	in; = 0 不显示;= 1 显示Open表;= 2显示close表;
  #返回值:
  #	SUCCESS	插入成功
  #	ERROR	插入失败  
  ***********************************************************************/
int OrderEnQueue(TYPE *Open,int *in,int *out,TYPE value,int mode,int showFlag){
	int temp,next;
	int i,tempIn,tempOut;

	tempIn = *in;
	tempOut = *out;
	temp = (tempIn+1)%MaxVertexNum;		//队满条件 in+1 = out

	if(temp == tempOut){
		printf("queue is fulled!\n");
		return ERROR;
	}
	i = tempIn;
	if(i < tempOut){			//解决循环队列的问题
		i = i + MaxVertexNum;
	}
	if(tempIn == tempOut){			//队空条件 in == out
		Open[tempIn] = value;
	}else{
		switch(mode)
		{

			case 0:	for( ;i > tempOut; i--){  	//按Cost插入
					next = tempIn - 1;
					if(next < 0){
						next = MaxVertexNum - 1;
					}
					if(value->cost < Open[next]->cost){
						Open[tempIn] = Open[next];
					}else{
						Open[tempIn] = value;
						break;
					}
					tempIn--;
					if(tempIn < 0){
						tempIn = MaxVertexNum - 1;
					}

				}
				if(i == tempOut){
					Open[tempIn] = value;
				}
				break;
			case 1:	for( ;i > tempOut; i--){	//按evaluate插入
					next = tempIn - 1;
					if(next < 0){
						next = MaxVertexNum - 1;
					}
					if(value->evaluate < Open[next]->evaluate){
						Open[tempIn] = Open[next];
					}else{
						Open[tempIn] = value;
						break;
					}
					tempIn--;
					if(tempIn < 0){
						tempIn = MaxVertexNum - 1;
					}
				}
				if(i == tempOut){
					Open[tempIn] = value;
				}
				break;

		}
	}

	(*in) = temp;
	if(showFlag == 1){
		showOpenaQueue(Open,(*in),(*out));
	}
	return SUCCESS;
}
/*************************************************************************
  #取出队首元素
  #参数说明:
  #	*Open  	元素所在的队列	in/out;
  #	*in 	队尾位置	in/out;
  #	*out 	队首位置	in/out;
  #	*value 	取出元素的值	out;
  #	showFlag显示标志	in; = 0 不显示;= 1 显示Open表;= 2显示close表;
  #返回值:
  #	SUCCESS	取出成功
  #	ERROR	取出失败  
  ***********************************************************************/
int GetQueue(TYPE *Open,int *in,int *out,TYPE *value,int showFlag){
	if((*in) == (*out)){		//队空条件 in == out
		return ERROR;
	}
	*value = Open[*out];
	(*out) = ((*out)+1)%MaxVertexNum;
	if(showFlag == 1){
		showOpenaQueue(Open,(*in),(*out));
	}
	return SUCCESS;
}


/*************************************************************************
  #宽度优先搜索算法
  #参数说明:
  #	start  	起始节点	in;
  #	target	目标节点	in; 
  #	showFlag显示标志	in; = 0 不显示;= 1 显示;
  #返回值:
  #	Result	搜索结果
  ***********************************************************************/
Result  BFS(char start ,char target,int showFlag){

	int n,findFlag,index,showC,showO,cost;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			 


	//初始化
	resetVisit();
	n = 0;
	cost = 0;
	findFlag = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	emptyOpen = emptyClose = 0;
	numOpen = numClose = 0;
	
	if(showFlag == 1){			//显示Open和 close表
		showO = 1;
		showC = 2;
	}else{					//不显示Open和 close表
		showO = 0;
		showC = 0;
	}

	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father = NULL;
	tempNode->cost = cost;
	EnQueue(Open,&inOpen,&outOpen,tempNode,showO);				//将起始节点存入open表
	numOpen++;

	while(1){
		emptyOpen = GetQueue(Open,&inOpen,&outOpen,&tempNode,showO);	//从open表中取出第一个值

		if(emptyOpen == -1){						//Open表为空,找不到目标节点,算法结束
			findFlag = 0;
			printf("BFS not find %c. \n",target);
			break;
		}

		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;

		searchResult.cost = tempNode->cost;
		if(G->adjlist[n].vertex == target){				//找到目标节点,算法结束
			findFlag = 1; 
			break;

		}

		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入Open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = tempNode->cost + cost;
				EnQueue(Open,&inOpen,&outOpen,tempNode,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
	searchResult.numOpen = numOpen;					//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	searchResult.inClose = inClose;
	searchResult.target = target;
	return searchResult;
}
 
/*************************************************************************
  #深度优先搜索算法
  #参数说明:
  #	start  	起始节点	in;
  #	target	目标节点	in; 
  #	showFlag显示标志	in; = 0 不显示;= 1 显示;
  #返回值:
  #	Result	搜索结果
  ***********************************************************************/
Result DFS(char start,char target,int showFlag){
	int n,cost,findFlag,index,showO,showC;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;

	//初始化
	resetVisit();
	n = 0;
	cost = 0;
	findFlag = 0;
	inOpen = inClose = 0;			 
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0; 
	
	if(showFlag == 1){		//显示Open和 close表
		showO = 1;
		showC = 2;
	}else{				//不显示Open和 close表
		showO = 0;
		showC = 0;
	}

	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father= NULL;
	tempNode->cost = cost;
	Push(Open,&inOpen,tempNode,showO);				//将起始节点存入open表
	numOpen++;
	while(1){
		emptyOpen = Pop(Open,&inOpen,&tempNode,showO);		//从open表中取出第一个值
		if(emptyOpen == -1){					//找到目标节点,算法结束
			findFlag = 0;
			printf("DFS not find %c. \n",target);
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);			//将该节点存入close表
		numClose++;

		searchResult.cost = tempNode->cost;
		if(G->adjlist[n].vertex == target){			//找到目标节点,算法结束
			findFlag = 1;
			break;

		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入Open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = tempNode->cost + cost;
				Push(Open,&inOpen,tempNode,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
	searchResult.numOpen = numOpen;					//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	searchResult.inClose = inClose;
	searchResult.target = target;
	return searchResult;
}
 
/*************************************************************************
  #有界深度优先搜索算法
  #参数说明:
  #	start  	起始节点	in;
  #	target	目标节点	in; 
  #	showFlag显示标志	in; = 0 不显示;= 1 显示;
  #返回值:
  #	Result	搜索结果
  ***********************************************************************/
Result DLS(char start,char target,int showFlag){
	int n,deep,cost,findFlag,index,showO,showC;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			
	

	//初始化
	resetVisit();
	if(showFlag == 1){
		showO = 1;
		showC = 2;
	}else{
		showO = 0;
		showC = 0;
	}

	n = 0;
	cost = 0;
	deep = 0;
	findFlag = 0;
	inOpen = inClose = 0;		
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;
	
	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father= NULL;
	tempNode->deep = deep;
	tempNode->cost = cost;
	Push(Open,&inOpen,tempNode,showO);					//将起始节点存入open表
	numOpen++;

	while(1){
		emptyOpen = Pop(Open,&inOpen,&tempNode,showO);			//从open表中取出第一个值
		if(emptyOpen == -1){						//Open表为空,找不到目标节点，算法结束
			findFlag = 0;
			printf("DLS not find %c. \n",target);
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;

		searchResult.cost = tempNode->cost;
		if(G->adjlist[n].vertex == target){				//是否为目标节点，算法结束
			findFlag = 1;
			break;

		}
		deep = tempNode->deep;					
		if(deep == DEEPLIMIT){					//深度是否等于深度限制
			continue;
		}
		deep = deep + 1;					//深度加 1
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入Open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->deep = deep;
				tempNode->cost = tempNode->cost + cost;
				Push(Open,&inOpen,tempNode,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
	searchResult.numOpen = numOpen;				//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	searchResult.inClose = inClose;
	searchResult.target = target;
	return searchResult;
}

/*************************************************************************
  #迭代法搜索算法
  #参数说明:
  #	start  	起始节点	in;
  #	target	目标节点	in; 
  #	showFlag显示标志	in; = 0 不显示;= 1 显示;
  #返回值:
  #	Result	搜索结果
  ***********************************************************************/
Result IDS(char start,char target,int showFlag){
	int num,n,deep,cost,index;
	int findFlag,limit,showO,showC;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;	
		
	//初始化
	if(showFlag == 1){
		showO = 1;
		showC = 2;
	}else{
		showO = 0;
		showC = 0;
	}

	n = 0;
	limit = 0;
	findFlag = 0;
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;
	

labelStar:
	num = 0;				//一次遍历节点的个数
	cost = 0;
	deep = 0;
	inOpen = inClose = 0;		 
	resetVisit();
	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father= NULL;
	tempNode->deep = deep;
	tempNode->cost = cost;
	Push(Open,&inOpen,tempNode,showO);				//将起始节点存入open表
	numOpen++;
	num++;

	while(1){
		emptyOpen = Pop(Open,&inOpen,&tempNode,showO);		//从open表中取出第一个值
		if(emptyOpen == -1){					//Open表为空,找不到目标节点
			if(num < G->n){ 			//没有遍历完所有节点，深度加 1，继续遍历
				limit++;
				goto labelStar;
			}else{					//遍历完所有节点，算法结束
				findFlag = 0;
				printf("IDS not find %c. \n",target);
				break;
			}
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;

		searchResult.cost = tempNode->cost;
		if(G->adjlist[n].vertex == target){				//是目标节点，算法结束
			findFlag = 1;
			break;
		}
		deep = tempNode->deep;					
		if(deep == limit){					//深度是否等于深度限制
			continue;
		}
		deep = deep + 1;					//深度加 1
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入Open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->deep = deep;
				tempNode->cost = tempNode->cost + cost;
				Push(Open,&inOpen,tempNode,showO);
				numOpen++;
				num++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
	searchResult.numOpen = numOpen;
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	searchResult.inClose = inClose;
	searchResult.target = target;
	return searchResult;
}

/*************************************************************************
  #等待价搜索算法
  #参数说明:
  #	start  	起始节点	in;
  #	target	目标节点	in; 
  #	showFlag显示标志	in; = 0 不显示;= 1 显示;
  #返回值:
  #	Result	搜索结果
  ***********************************************************************/
Result  UCS(char start ,char target,int showFlag){
	int n,cost,findFlag,index,showO,showC;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n
	
	if(showFlag == 1){
		showO = 1;
		showC = 2;
	}else{
		showO = 0;
		showC = 0;
	}

	n = 0;
	cost = 0;
	findFlag = 0;
	resetVisit();
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;

	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->cost = cost;
	tempNode->father = NULL;
	OrderEnQueue(Open,&inOpen,&outOpen,tempNode,0,showO);//将第一个节点的下一个节点存入open表
	numOpen++;
	while(1){
		emptyOpen = GetQueue(Open,&inOpen,&outOpen,&tempNode,showO);	//从open表中取出第一个值
		if(emptyOpen == -1){						//Open表为空,找不到目标节点，算法结束
			findFlag = 0;
			printf("UCS not find %c. \n",target);
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;
		
		searchResult.cost = tempNode->cost;
		if(G->adjlist[n].vertex == target){				//是目标节点，算法结束
			findFlag = 1;
			break;
		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入Open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = cost + tempNode->cost;
				OrderEnQueue(Open,&inOpen,&outOpen,tempNode,0,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
	searchResult.numOpen = numOpen;			//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	searchResult.inClose = inClose;
	searchResult.target = target;
	return searchResult;
}
 
/*************************************************************************
  #最佳优先搜索算法
  #参数说明:
  #	start  	起始节点	in;
  #	target	目标节点	in; 
  #	showFlag显示标志	in; = 0 不显示;= 1 显示;
  #返回值:
  #	Result	搜索结果
  ***********************************************************************/
Result  BestFS(char start,char target,int showFlag){
	int n,cost,findFlag,index,showO,showC;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			 
	
	//初始化
	if(showFlag == 1){
		showO = 1;
		showC = 2;
	}else{
		showO = 0;
		showC = 0;
	}

	resetVisit();
	n = 0;
	cost = 0;
	findFlag = 0;
	inOpen = inClose = 0;			 
	outOpen = outClose = 0;
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;

	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->cost = cost;
	tempNode->father = NULL;
	OrderEnQueue(Open,&inOpen,&outOpen,tempNode,1,showO);//将起始节点存入open表
	numOpen++;

	while(1){
		emptyOpen = GetQueue(Open,&inOpen,&outOpen,&tempNode,showO);	//从open表中取出第一个值
		if(emptyOpen == -1){						//Open表为空,找不到目标节点，算法结束
			findFlag = 0;
			printf("BestFS not find %c. \n",target);
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;
		searchResult.cost = tempNode->cost;
		if(G->adjlist[n].vertex == target){				//是目标节点，算法结束
			findFlag = 1;
			break;
		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;				//将后继节点存入Open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = cost + tempNode->cost;
				tempNode->evaluate = tempNode->cost + tempNode->evaluate;
			n = tempNode->adjvex;
				OrderEnQueue(Open,&inOpen,&outOpen,tempNode,1,showO);
			n = tempNode->adjvex;
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
	searchResult.numOpen = numOpen;			//返回结果 
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	searchResult.inClose = inClose;
	searchResult.target = target;
	return searchResult;
}


//建立测试文件
void CreateFile(){
	int 	fp;
	char	buffer[] = "顶点数 边数\n6 7\n顶点名称\na b c d e f\n起始顶点编号 	终点编号	代价值	估价值\n0 1 1 6\n0 2 1 7\n1 3 1 1\n2 3 1 1\n2 4 1 1\n3 4 1 1\n4 5 1 1";
	fp = open("./testData.txt",O_WRONLY|O_CREAT,S_IRUSR|S_IWUSR);
	write(fp,buffer,sizeof(buffer));
	close(fp);
}
