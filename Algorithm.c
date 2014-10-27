#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include"GUI.h"

#define MaxVertexNum 	50        	//定义最大顶点数
#define SUCCESS		0		//函数执行成功
#define FAILURE 	-1		//函数执行失败
#define ERROR		-1		//函数执行失败
//#define TRUE 		1		//已经遍历过
//#define FALSE 		0		//未遍历
//#define DEEPLIMIT 	2		//有界深度优先搜索算法的 深度界限

extern	GtkWidget 	*mainwindow;
extern 	GtkWidget 	*Tstart,*Tend; 
extern	GtkWidget 	*Ffile;
extern	GtkWidget  	*Topen,*Tclose;
extern	GtkTextBuffer	*bufopen,*bufclose;
typedef EdgeNode*  TYPE;

int DEEPLIMIT = 2;
EdgeNode*  open[MaxVertexNum]; 			//open表
EdgeNode*  Close[MaxVertexNum];			//close表
Result	   searchResult;
ALGraph *G;

int Visit[MaxVertexNum] = {FALSE};

//显示邻接点表 
void show(){
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
void CreatALGraph(const char fileName[])
{
	int i,j,k,cost,evaluate;
	char a;
	EdgeNode *s;       				 //定义边表结点
	printf("open file is %s\n",fileName);
	FILE *fp;
	if((fp = fopen(fileName,"r")) == NULL){
		perror("open file error\n");
		return;
	}
	fscanf(fp,"%d%d",&i,&j);			 //读入顶点数和边数
	G->n=i;G->e=j;
	fgetc(fp);

	for(i=0;i<G->n;i++)   				 //建立边表
	{
		fscanf(fp,"%c",&a);
		fgetc(fp);
		G->adjlist[i].vertex=a;    		//读入顶点信息
		G->adjlist[i].firstedge=NULL; 		//边表置为空表
	}

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
	show();			//打印邻接点表
}

//查询元素在边表的值
int valueIndex(char value){
	int index;
	for(index = 0;index < (G->n); index++){
		if(G->adjlist[index].vertex == value){
			break;
		}
	}
	return index;
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
   #	*open  	要插入的表	in/out;
   #	*in 	插入位置	in/out;
   #	value 	插入的值	in;
   #	showFlag显示标志	in; = 0 不显示;= 1 显示open表;= 2显示close表;
   #返回值:
   #	SUCCESS	插入成功
   #	ERROR	插入失败  
  ***********************************************************************/
int Push(TYPE *open,int *in,TYPE value,int showFlag){
	if((*in) > MaxVertexNum){		//表满
		printf("stack is fulled!\n");
		return ERROR;
	}
	open[*in] = value;
	(*in)++;
	switch(showFlag){
		case 1:	showOpenaStack(open,(*in));break;
		case 2:	showClose(open, (*in));break;

	}
	return SUCCESS;
}
/*************************************************************************
  #取出表头元素
  #参数说明:
  #	*open  	元素所在的表	in/out;
  #	*out 	取出元素位置	in/out;
  #	*value 	返回元素的值	out;
  #	showFlag显示标志	in; = 0 不显示;= 1 显示open表;= 2显示close表;
  #返回值:
  #	SUCCESS	取出成功
  #	ERROR	取出失败  
  ***********************************************************************/
int Pop(TYPE *open,int *out,TYPE *value,int showFlag){
	if((*out) <= 0){
		return ERROR;
	}
	(*out)--;
	*value = open[*out];
	switch(showFlag){
		case 1:	showOpenaStack(open,(*out));break;
		case 2:	showClose(open,(*out));break;
	}
	return SUCCESS;
}
/*************************************************************************
  #将元素插入队尾
  #参数说明:
  #	*open  	要插入的队列	in/out;
  #	*in 	队尾位置	in/out;
  #	*out 	队首位置	in/out;
  #	value 	插入的值	in;
  #	showFlag显示标志	in; = 0 不显示;= 1 显示open表;= 2显示close表;
  #返回值:
  #	SUCCESS	插入成功
  #	ERROR	插入失败  
  ***********************************************************************/
int EnQueue(TYPE *open,int *in,int *out,TYPE value,int showFlag){
	int temp;
	temp = ((*in)+1)%MaxVertexNum;		//队满条件 in+1 = out
	if(temp == (*out)){
		printf("queue is fulled!\n");
		return ERROR;
	}
	open[*in] = value;
	(*in) = temp;
	if(showFlag == 1){
		showOpenaQueue(open,(*in),(*out));
	}
	return SUCCESS;
}
/*************************************************************************
  #将元素插入队尾
  #参数说明:
  #	*open  	要插入的队列	in/out;
  #	*in 	队尾位置	in/out;
  #	*out 	队首位置	in/out;
  #	value 	插入的值	in;
  # 	mode	排序模式	in; = 0 按cost排序; = 1 按evaluate排序
  #	showFlag显示标志	in; = 0 不显示;= 1 显示open表;= 2显示close表;
  #返回值:
  #	SUCCESS	插入成功
  #	ERROR	插入失败  
  ***********************************************************************/
int OrderEnQueue(TYPE *open,int *in,int *out,TYPE value,int mode,int showFlag){
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
		open[tempIn] = value;
	}else{
		switch(mode)
		{

			case 0:	for( ;i > tempOut; i--){  	//按Cost插入
					next = tempIn - 1;
					if(next < 0){
						next = MaxVertexNum - 1;
					}
					if(value->cost < open[next]->cost){
						open[tempIn] = open[next];
					}else{
						open[tempIn] = value;
						break;
					}
					tempIn--;
					if(tempIn < 0){
						tempIn = MaxVertexNum - 1;
					}

				}
				if(i == tempOut){
					open[tempIn] = value;
				}
				break;
			case 1:	for( ;i > tempOut; i--){	//按evaluate插入
					next = tempIn - 1;
					if(next < 0){
						next = MaxVertexNum - 1;
					}
					if(value->evaluate < open[next]->evaluate){
						open[tempIn] = open[next];
					}else{
						open[tempIn] = value;
						break;
					}
					tempIn--;
					if(tempIn < 0){
						tempIn = MaxVertexNum - 1;
					}
				}
				if(i == tempOut){
					open[tempIn] = value;
				}
				break;

		}
	}

	(*in) = temp;
	if(showFlag == 1){
		showOpenaQueue(open,(*in),(*out));
	}
	return SUCCESS;
}
/*************************************************************************
  #取出队首元素
  #参数说明:
  #	*open  	元素所在的队列	in/out;
  #	*in 	队尾位置	in/out;
  #	*out 	队首位置	in/out;
  #	*value 	取出元素的值	out;
  #	showFlag显示标志	in; = 0 不显示;= 1 显示open表;= 2显示close表;
  #返回值:
  #	SUCCESS	取出成功
  #	ERROR	取出失败  
  ***********************************************************************/
int GetQueue(TYPE *open,int *in,int *out,TYPE *value,int showFlag){
	if((*in) == (*out)){		//队空条件 in == out
		return ERROR;
	}
	*value = open[*out];
	(*out) = ((*out)+1)%MaxVertexNum;
	if(showFlag == 1){
		showOpenaQueue(open,(*in),(*out));
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

	int n,findFlag,index,showC,showO;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			 


	//初始化
	resetVisit();
	n = 0;
	findFlag = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	emptyOpen = emptyClose = 0;
	numOpen = numClose = 0;
	
	if(showFlag == 1){			//显示open和 close表
		showO = 1;
		showC = 2;
	}else{					//不显示open和 close表
		showO = 0;
		showC = 0;
	}

	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father = NULL;
	EnQueue(open,&inOpen,&outOpen,tempNode,showO);				//将起始节点存入open表
	numOpen++;

	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode,showO);	//从open表中取出第一个值

		if(emptyOpen == -1){						//open表为空,找不到目标节点,算法结束
			findFlag = 0;
			printf("BFS not find %c. \n",target);
			break;
		}

		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;

		if(G->adjlist[n].vertex == target){				//找到目标节点,算法结束
			findFlag = 1; 

			do{							//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode,0);
				if(emptyClose != -1 && tempNode->adjvex == n){
					n = tempNode->adjvex;
					printf("BFS Close element : %c\n",G->adjlist[n].vertex);
					tempNode= tempNode->father;
					if(tempNode == NULL){
						break;
					}
					n = tempNode->adjvex;
				}
			}while(emptyClose != -1);

			printf("BFS find %c. \n",target);
			break;
		}

		tempNode = G->adjlist[n].firstedge;			//将后继节点存入open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				EnQueue(open,&inOpen,&outOpen,tempNode,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
	searchResult.numOpen = numOpen;					//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
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
	resetVisit();
	int n,findFlag,index,showO,showC;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;

	//初始化
	n = 0;
	findFlag = 0;
	inOpen = inClose = 0;			 
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0; 
	
	if(showFlag == 1){		//显示open和 close表
		showO = 1;
		showC = 2;
	}else{				//不显示open和 close表
		showO = 0;
		showC = 0;
	}

	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father= NULL;
	Push(open,&inOpen,tempNode,showO);				//将起始节点存入open表
	numOpen++;
	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode,showO);		//从open表中取出第一个值
		if(emptyOpen == -1){					//找到目标节点,算法结束
			findFlag = 0;
			printf("DFS not find %c. \n",target);
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);			//将该节点存入close表
		numClose++;
		if(G->adjlist[n].vertex == target){			//找到目标节点,算法结束
			findFlag = 1;
			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode,0);
				if(emptyClose != -1 && tempNode->adjvex == n){
					n = tempNode->adjvex;
					printf("DFS Close element : %c\n",G->adjlist[n].vertex);
					tempNode= tempNode->father;
					if(tempNode == NULL){
						break;
					}
					n = tempNode->adjvex;
				}
			}while(emptyClose != -1);
			printf("DFS fine %c. \n",target);
			break;
		}
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				Push(open,&inOpen,tempNode,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
	searchResult.numOpen = numOpen;					//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
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
Result LimitDFS(char start,char target,int showFlag){
	resetVisit();
	int n,deep,findFlag,index,showO,showC;
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
	Push(open,&inOpen,tempNode,showO);					//将起始节点存入open表
	numOpen++;

	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode,showO);			//从open表中取出第一个值
		if(emptyOpen == -1){						//open表为空,找不到目标节点，算法结束
			findFlag = 0;
			printf("LimitDFS not find %c. \n",target);
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;

		if(G->adjlist[n].vertex == target){				//是否为目标节点，算法结束
			findFlag = 1;
			do{							//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode,0);
				if(emptyClose != -1 && tempNode->adjvex == n){
					n = tempNode->adjvex;
					printf("LimitDFS Close element : %c\n",G->adjlist[n].vertex);
					tempNode= tempNode->father;
					if(tempNode == NULL){
						break;
					}
					n = tempNode->adjvex;
				}
			}while(emptyClose != -1);
			printf("LimitDFS fine %c. \n",target);
			break;
		}
		deep = tempNode->deep;					
		if(deep == DEEPLIMIT){					//深度是否等于深度限制
			continue;
		}
		deep = deep + 1;					//深度加 1
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->deep = deep;
				Push(open,&inOpen,tempNode,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
	searchResult.numOpen = numOpen;				//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
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
Result IterDFS(char start,char target,int showFlag){
	int num,n,deep,findFlag,index,limit,showO,showC;
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
	deep = 0;
	inOpen = inClose = 0;		 
	resetVisit();
	index = valueIndex(start);
	tempNode = G->adjlist[index].firstedge;
	Visit[index] = TRUE;
	tempNode->father= NULL;
	tempNode->deep = deep;
	Push(open,&inOpen,tempNode,showO);				//将起始节点存入open表
	numOpen++;
	num++;

	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode,showO);		//从open表中取出第一个值
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			if(num < G->n){ 			//没有遍历完所有节点，深度加 1，继续遍历
				limit++;
				goto labelStar;
			}else{					//遍历完所有节点，算法结束
				findFlag = 0;
				printf("IterDFS not find %c. \n",target);
				break;
			}
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;

		if(G->adjlist[n].vertex == target){				//是目标节点，算法结束
			findFlag = 1;
			do{							//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode,0);
				if(emptyClose != -1 && tempNode->adjvex == n){
					n = tempNode->adjvex;
					printf("IterDFS Close element : %c\n",G->adjlist[n].vertex);
					tempNode= tempNode->father;
					if(tempNode == NULL){
						break;
					}
					n = tempNode->adjvex;
				}
			}while(emptyClose != -1);
			printf("IterDFS fine %c. \n",target);
			break;
		}
		deep = tempNode->deep;					
		if(deep == limit){					//深度是否等于深度限制
			continue;
		}
		deep = deep + 1;					//深度加 1
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->deep = deep;
				Push(open,&inOpen,tempNode,showO);
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
Result  CostSearch(char start ,char target,int showFlag){
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
	OrderEnQueue(open,&inOpen,&outOpen,tempNode,0,showO);//将第一个节点的下一个节点存入open表
	numOpen++;
	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode,showO);	//从open表中取出第一个值
		if(emptyOpen == -1){						//open表为空,找不到目标节点，算法结束
			findFlag = 0;
			printf("CostSearch not find %c. \n",target);
			searchResult.cost = tempNode->cost;
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE; 
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;
		if(G->adjlist[n].vertex == target){				//是目标节点，算法结束
			findFlag = 1;
			printf("CostSearch find %c and cost %d.  \n",target,tempNode->cost);
			searchResult.cost = tempNode->cost;
			do{							//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode,0);
				if(emptyClose != -1 && tempNode->adjvex == n){
					n = tempNode->adjvex;
					printf("CostSearch Close element : %c\n",G->adjlist[n].vertex);
					tempNode= tempNode->father;
					if(tempNode == NULL){
						break;
					}
					n = tempNode->adjvex;
				}
			}while(emptyClose != -1);
			break;
		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//将后继节点存入open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = cost + tempNode->cost;
				OrderEnQueue(open,&inOpen,&outOpen,tempNode,0,showO);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
	searchResult.numOpen = numOpen;			//返回结果
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
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
Result  BestSearch(char start,char target,int showFlag){
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
	OrderEnQueue(open,&inOpen,&outOpen,tempNode,1,showO);//将起始节点存入open表
	numOpen++;

	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode,showO);	//从open表中取出第一个值
		if(emptyOpen == -1){						//open表为空,找不到目标节点，算法结束
			findFlag = 0;
			printf("BestSearch not find %c. \n",target);
			searchResult.cost = tempNode->cost;
			break;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when BestSearchfind the %d element : %c cost %d\n",n,G->adjlist[n].vertex,tempNode->cost);
		Push(Close,&inClose,tempNode,showC);				//将该节点存入close表
		numClose++;
		if(G->adjlist[n].vertex == target){				//是目标节点，算法结束
			findFlag = 1;
			printf("BestSearch find %c and cost %d.  \n",target,tempNode->cost);
			searchResult.cost = tempNode->cost;
			do{							//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode,0);
				if(emptyClose != -1 && tempNode->adjvex == n){
					n = tempNode->adjvex;
					printf("BestSearch Close element : %c\n",G->adjlist[n].vertex);
					tempNode= tempNode->father;
					if(tempNode == NULL){
						break;
					}
					n = tempNode->adjvex;
				}
			}while(emptyClose != -1);
			break;
		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;				//将后继节点存入open表
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			printf("1%c\t",G->adjlist[n].vertex);
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = cost + tempNode->cost;
				tempNode->evaluate = tempNode->cost + tempNode->evaluate;
			n = tempNode->adjvex;
			printf("2%c\t",G->adjlist[n].vertex);
				OrderEnQueue(open,&inOpen,&outOpen,tempNode,1,showO);
			n = tempNode->adjvex;
			printf("3%c\t\n",G->adjlist[n].vertex);
				numOpen++;
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
	searchResult.numOpen = numOpen;			//返回结果 
	searchResult.numClose = numClose;
	searchResult.findFlag = findFlag;
	return searchResult;
}
