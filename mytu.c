#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>
#include"GUI.h"

#define MaxVertexNum 	50        	//定义最大顶点数
#define SUCCESS		0		//函数执行成功
#define FAILURE 	-1		//函数执行失败
#define ERROR		-1		//函数执行失败
#define TRUE 		1		//已经遍历过
#define FALSE 		0		//未遍历
#define DEEPLIMIT 	2		//有界深度优先搜索算法的 深度界限

extern	GtkWidget 	*mainwindow;
extern 	GtkWidget 	*Tstart,*Tend; 
extern	GtkWidget 	*Ffile;
extern	GtkWidget  	*Topen,*Tclose;
extern	GtkTextBuffer	*bufopen,*bufclose;
typedef EdgeNode*  TYPE;

EdgeNode*  open[MaxVertexNum]; //open表，next指向父节点
EdgeNode*  Close[MaxVertexNum];//close表，next指向父节点，下标表示编号
int Visit[MaxVertexNum] = {FALSE};
//           建立图的邻接表           
void CreatALGraph(ALGraph *G,char fileName[])
{
	int i,j,k,cost,evaluate;
	char a;
	EdgeNode *s;        //定义边表结点
	FILE *fp;
	if((fp = fopen(fileName,"r")) == NULL){
		perror("open file error\n");
		return;
	}
//	printf("请输入顶点数和边数: ");
//	scanf("%d%d",&i,&j); //读入顶点数和边数
	fscanf(fp,"%d%d",&i,&j); //读入顶点数和边数
	G->n=i;G->e=j;
//	printf("图的顶点数和变数分别为：%d,%d\n",G->n,G->e);
//	getchar();
	fgetc(fp);
//	printf("请输入顶点编号:\n");
	for(i=0;i<G->n;i++)     //建立边表
	{
//		printf("第 %d 个顶点的编号为：",i);
	//	scanf("%c",&a);
		fscanf(fp,"%c",&a);
	fgetc(fp);
	//	getchar();
		G->adjlist[i].vertex=a;      //读入顶点信息
		G->adjlist[i].firstedge=NULL; //边表置为空表
	}
//	printf("请输入由两个顶点构成的边，示例：0 1\n");
	for(k=0;k<G->e;k++) 
	{ //建立边表
		//cin>>i;cin>>j;        //读入边（Vi，Vj）的顶点对序号
//		scanf("%d%d%d",&i,&j,&cost);
		fscanf(fp,"%d%d%d%d",&i,&j,&cost,&evaluate);
	fgetc(fp);
		s=(EdgeNode *)malloc(sizeof(EdgeNode));    //生成边表结点
		s->adjvex=j;                 //邻接点序号为j
		s->cost = cost;
		s->evaluate = evaluate;
		s->next=G->adjlist[i].firstedge;
		G->adjlist[i].firstedge=s;     //将新结点*S插入顶点Vi的边表头部
		s=(EdgeNode *)malloc(sizeof(EdgeNode)); 
		s->adjvex=i;                  //邻接点序号为i
		s->cost = cost;
		s->evaluate = evaluate;
		s->next=G->adjlist[j].firstedge;   
		G->adjlist[j].firstedge=s;      //将新结点*S插入顶点Vj的边表头部
	}
	for(k=0;k<G->n;k++){
		s=(EdgeNode *)malloc(sizeof(EdgeNode));    //生成边表结点
		s->adjvex=k;                 //邻接点序号为j
		s->next=G->adjlist[k].firstedge;
		G->adjlist[k].firstedge=s;     //将新结点*S插入顶点Vi的边表头部
	}
}

//显示邻接点表 
void show(ALGraph *G){
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

int Push(TYPE *open,int *in,TYPE value){
	if((*in) > MaxVertexNum){
		printf("stack is fulled!\n");
		return ERROR;
	}
	open[*in] = value;
	(*in)++;
	return SUCCESS;
}
int Pop(TYPE *open,int *out,TYPE *value){
	if((*out) <= 0){
		return ERROR;
	}
	(*out)--;
	*value = open[*out];
	return SUCCESS;
}
//入队列
//参数说明：open 数组，in 队首指针，out 队尾指针，value 入队的值
int EnQueue(TYPE *open,int *in,int *out,TYPE value){
	int temp;
	temp = ((*in)+1)%MaxVertexNum;		//队满条件 in+1 = out
	if(temp == (*out)){
		printf("queue is fulled!\n");
		return ERROR;
	}
	open[*in] = value;
	(*in) = temp;
	return SUCCESS;
}

//有序的入队列
//根据cost排序
int OrderEnQueue(TYPE *open,int *in,int *out,TYPE value,int mode){
	TYPE tempNode;
	int temp;
	int i;
	temp = ((*in)+1)%MaxVertexNum;		//队满条件 in+1 = out
	if(temp == (*out)){
		printf("queue is fulled!\n");
		return ERROR;
	}
	i = (*in);
	if(i < (*out)){				//解决循环队列的问题
		i = i + MaxVertexNum;
	}
	if((*in) == (*out)){			//队空条件 in == out
		open[i] = value;
	}else{
		switch(mode){
	
			case 0:	for(i ;i > (*out); i--){
					if(value->cost < open[i-1]->cost){
						open[i] = open[i-1];
					}else{
						open[i] = value;
					}

				}
			case 1:	for(i ;i > (*out); i--){
					if(value->evaluate < open[i-1]->evaluate){
						open[i] = open[i-1];
					}else{
						open[i] = value;
					}
				}

		}
	}

	(*in) = temp;
	return SUCCESS;
}

//出队列
//参数说明：open 数组，in 队首指针，out 队尾指针
int GetQueue(TYPE *open,int *in,int *out,TYPE *value){
	if((*in) == (*out)){		//队空条件 in == out
		return ERROR;
	}
	*value = open[*out];
	(*out) = ((*out)+1)%MaxVertexNum;
	return SUCCESS;
}


//宽度优先搜索算法

int  BFS(ALGraph *G,char value){
	int i,n,findFlag;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	n = 0;
	memset(Visit,0,sizeof(Visit));
	findFlag = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	emptyOpen = emptyClose = 0;
	numOpen = numClose = 0;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n

	tempNode = G->adjlist[0].firstedge;
	Visit[0] = TRUE;
	tempNode->father = NULL;
	EnQueue(open,&inOpen,&outOpen,tempNode);//将第一个节点的下一个节点存入open表
	numOpen++;
	showOpenaQueue(open, G,inOpen,outOpen);

	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode);	//从open表中取出第一个值
		showOpenaQueue(open, G,inOpen,outOpen);

		if(emptyOpen == -1){					//open表为空,找不到目标节点
			findFlag = 0;
			showResult(findFlag,numOpen,numClose);
			printf("BFS not find %c. \n",value);
			return ERROR;
		}

		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when BFS find the %d element : %c\n",n,G->adjlist[n].vertex);
		Push(Close,&inClose,tempNode);		//将该节点存入close表
		numClose++;
		showClose(Close, G,inClose);

		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			findFlag = 1;
			printf("BFS.........1\n");
			printf("findFlag %d,numOpen %d,numClose %d\n",findFlag,numOpen,numClose);
			showResult(findFlag,numOpen,numClose);
			printf("BFS.........2\n");

			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode);
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

			printf("BFS find %c. \n",value);
			return SUCCESS; 	
		}

		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				EnQueue(open,&inOpen,&outOpen,tempNode);
				numOpen++;
				showOpenaQueue(open, G,inOpen,outOpen);
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
}

//深度优先搜索

int DFS(ALGraph *G,char value){
	memset(Visit,0,sizeof(Visit));
	int i,n,findFlag;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	n = 0;
	findFlag = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n
	tempNode = G->adjlist[0].firstedge;
	Visit[0] = TRUE;
	tempNode->father= NULL;
	Push(open,&inOpen,tempNode);//将第一个节点的下一个节点存入open表
	numOpen++;
	showOpenaStack(open,G,inOpen);
	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode);	//从open表中取出第一个值
		showOpenaStack(open,G,inOpen);
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			findFlag = 0;
			showResult(findFlag,numOpen,numClose);
			printf("DFS not find %c. \n",value);
			return ERROR;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when DFS find the %d element : %c\n",n,G->adjlist[n].vertex);
		Push(Close,&inClose,tempNode);		//将该节点存入close表
		numClose++;
		showClose(Close,G,inClose);
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			findFlag = 1;
			showResult(findFlag,numOpen,numClose);
			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode);
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
			printf("DFS fine %c. \n",value);
			return SUCCESS; 	
		}
		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				Push(open,&inOpen,tempNode);
				numOpen++;
				showOpenaStack(open,G,inOpen);
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
}


//有界深度优先搜索算法

int LimitDFS(ALGraph *G,char value){
	memset(Visit,0,sizeof(Visit));
	int i,n,deep,findFlag;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n

	n = 0;
	deep = 0;
	findFlag = 0;
	inOpen = inClose = 0;		//初始化队列的队首和队尾
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;
	
	tempNode = G->adjlist[0].firstedge;
	Visit[0] = TRUE;
	tempNode->father= NULL;
	tempNode->deep = deep;
	Push(open,&inOpen,tempNode);				//将第一个节点的下一个节点存入open表
	numOpen++;
	showOpenaStack(open,G,inOpen);
	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode);	//从open表中取出第一个值
		showOpenaStack(open,G,inOpen);
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			findFlag = 0;
			showResult(findFlag,numOpen,numClose);
			printf("LimitDFS not find %c. \n",value);
			return ERROR;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when LimitDFS find the %d element : %c\n",n,G->adjlist[n].vertex);
		Push(Close,&inClose,tempNode);				//将该节点存入close表
		numClose++;
		showClose(Close,G,inClose);
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			findFlag = 1;
			showResult(findFlag,numOpen,numClose);
			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode);
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
			printf("LimitDFS fine %c. \n",value);
			return SUCCESS; 	
		}
		deep = tempNode->deep;					
		if(deep == DEEPLIMIT){					//深度是否等于深度限制
			continue;
		}
		deep = deep + 1;					//深度加 1
		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->deep = deep;
				Push(open,&inOpen,tempNode);
				numOpen++;
				showOpenaStack(open,G,inOpen);
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
}


//迭代加深

int IterDFS(ALGraph *G,char value){
	//memset(open,NULL,sizeof(MaxVertexNum));//初始化open和Close表
	//memset(Close,NULL,sizeof(MaxVertexNum));
	memset(Visit,0,sizeof(Visit));
	int i,n,deep,findFlag;
	int inOpen,emptyOpen,numOpen;
	int inClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n

	n = 0;
	deep = 0;
	findFlag = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;

	tempNode = G->adjlist[0].firstedge;
	Visit[0] = TRUE;
	tempNode->father= NULL;
	tempNode->deep = deep;
	Push(open,&inOpen,tempNode);				//将第一个节点的下一个节点存入open表
	numOpen++;
	showOpenaStack(open,G,inOpen);

	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode);	//从open表中取出第一个值
		showOpenaStack(open,G,inOpen);
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			findFlag = 0;
			showResult(findFlag,numOpen,numClose);
			printf("IterDFS not find %c. \n",value);
			return ERROR;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when IterDFS find the %d element : %c\n",n,G->adjlist[n].vertex);
		Push(Close,&inClose,tempNode);				//将该节点存入close表
		numClose++;
		showClose(open,G,inClose);

		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			findFlag = 1;
			printf("Iter.......1\n");
			showResult(findFlag,numOpen,numClose);
			printf("Iter.......2\n");
			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode);
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
			printf("IterDFS fine %c. \n",value);
			return SUCCESS; 	
		}
		deep = tempNode->deep;					
		if(deep == DEEPLIMIT){					//深度是否等于深度限制
			continue;
		}
		deep = deep + 1;					//深度加 1
		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->deep = deep;
				Push(open,&inOpen,tempNode);
				numOpen++;
				showOpenaStack(open,G,inOpen);
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
}
//代价图搜索
int  CostSearch(ALGraph *G,char value){
	//memset(open,NULL,sizeof(MaxVertexNum));//初始化open和Close表
	//memset(Close,NULL,sizeof(MaxVertexNum));
	int i,n,cost,findFlag;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n

	n = 0;
	cost = 0;
	findFlag = 0;
	memset(Visit,0,sizeof(Visit));
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;

	tempNode = G->adjlist[0].firstedge;
	Visit[0] = TRUE;
	tempNode->cost = cost;
	tempNode->father = NULL;
	OrderEnQueue(open,&inOpen,&outOpen,tempNode,0);//将第一个节点的下一个节点存入open表
	numOpen++;
	showOpenaQueue(open,G,inOpen,outOpen);
	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode);	//从open表中取出第一个值
		showOpenaQueue(open,G,inOpen,outOpen);
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			findFlag = 0;
			showResult(findFlag,numOpen,numClose);
			printf("CostSearch not find %c. \n",value);
			return ERROR;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when CostSearchfind the %d element : %c cost %d\n",n,G->adjlist[n].vertex,tempNode->cost);
		Push(Close,&inClose,tempNode);		//将该节点存入close表
		numClose++;
		showClose(open,G,inClose);
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			findFlag = 1;
			showResult(findFlag,numOpen,numClose);
			printf("CostSearch find %c and cost %d.  \n",value,tempNode->cost);
			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode);
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
			return SUCCESS; 	
		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = cost + tempNode->cost;
				OrderEnQueue(open,&inOpen,&outOpen,tempNode,0);
				numOpen++;
				showOpenaQueue(open,G,inOpen,outOpen);
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
}

//最佳优先

int  BestSearch(ALGraph *G,char value){
	int i,n,cost,evaluate,findFlag;
	int inOpen,outOpen,emptyOpen,numOpen;
	int inClose,outClose,emptyClose,numClose;
	EdgeNode  *tempNode,*fatherNode;			//临时节点 n

	n = 0;
	cost = 0;
	findFlag = 0;
	evaluate = 0;
	memset(Visit,0,sizeof(Visit));
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	numOpen = numClose = 0;
	emptyOpen = emptyClose = 0;

	tempNode = G->adjlist[0].firstedge;
	Visit[0] = TRUE;
	tempNode->cost = cost;
	tempNode->father = NULL;
	OrderEnQueue(open,&inOpen,&outOpen,tempNode,1);//将第一个节点的下一个节点存入open表
	numOpen++;
	showOpenaQueue(open,G,inOpen,outOpen);

	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode);	//从open表中取出第一个值
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			findFlag = 0;
			showResult(findFlag,numOpen,numClose);
			printf("BestSearch not find %c. \n",value);
			return ERROR;
		}
		n = tempNode->adjvex;
		Visit[n] = TRUE;
		printf("when BestSearchfind the %d element : %c cost %d\n",n,G->adjlist[n].vertex,tempNode->cost);
		Push(Close,&inClose,tempNode);		//将该节点存入close表
		numClose++;
		showClose(Close, G,inClose);
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			findFlag = 1;
			showResult(findFlag,numOpen,numClose);
			printf("BestSearch find %c and cost %d.  \n",value,tempNode->cost);
			do{						//输出Close表中所有的节点
				emptyClose = Pop(Close,&inClose,&tempNode);
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
			return SUCCESS; 	
		}
		cost = tempNode->cost;
		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		fatherNode = tempNode;
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(Visit[n] == FALSE){
				tempNode->father = fatherNode;
				tempNode->cost = cost + tempNode->cost;
				tempNode->evaluate = tempNode->cost + tempNode->evaluate;
				OrderEnQueue(open,&inOpen,&outOpen,tempNode,1);
				numOpen++;
				showOpenaQueue(open,G,inOpen,outOpen);
				Visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
}

/*

int main(){
	ALGraph G;
	char find = 'e';
	char fileName[27] = "a.txt";
	CreatALGraph(&G,fileName);
	show(&G);
	DFS(&G,find);
	BFS(&G,find);
	LimitDFS(&G,find);
	CostSearch(&G,find);
	BestSearch(&G,find);
	return 0;
}
*/
