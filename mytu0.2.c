#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define MaxVertexNum 50        	//定义最大顶点数
#define SUCCESS	0		//函数执行成功
#define FAILURE -1		//函数执行失败
#define ERROR	-1		//函数执行失败
#define TRUE 1			//已经遍历过
#define FALSE 0			//未遍历
#define DEEPLIMIT 5		//有界深度优先搜索算法的 深度界限

typedef struct node{       //边表结点

	char adjvex;          //邻接点域

	struct node *next,*father;    //链域

	int deep;

}EdgeNode;

typedef struct vnode{     //顶点表结点

	char vertex;           //顶点域

	EdgeNode *firstedge; //边表头指针

}VertexNode;

typedef VertexNode AdjList[MaxVertexNum];       //AdjList是邻接表类型

typedef struct { 

	AdjList adjlist;     //邻接表

	int n,e;             //图中当前顶点数和边数

} ALGraph;               //图类型

//           建立图的邻接表           

void CreatALGraph(ALGraph *G)

{

	int i,j,k;

	char a;

	EdgeNode *s;        //定义边表结点

	printf("请输入顶点数和边数: ");

	scanf("%d%d",&i,&j); //读入顶点数和边数

	G->n=i;G->e=j;
	printf("图的顶点数和变数分别为：%d,%d\n",G->n,G->e);
	getchar();
	printf("请输入顶点编号:\n");

	for(i=0;i<G->n;i++)     //建立边表

	{
		printf("第 %d 个顶点的编号为：",i);

		scanf("%c",&a);
		getchar();
		G->adjlist[i].vertex=a;      //读入顶点信息

		G->adjlist[i].firstedge=NULL; //边表置为空表

	}

	printf("请输入由两个顶点构成的边，示例：0 1\n");

	for(k=0;k<G->e;k++) 

	{ //建立边表

		//cin>>i;cin>>j;        //读入边（Vi，Vj）的顶点对序号
		scanf("%d%d",&i,&j);
		s=(EdgeNode *)malloc(sizeof(EdgeNode));    //生成边表结点

		s->adjvex=j;                 //邻接点序号为j

		s->next=G->adjlist[i].firstedge;

		G->adjlist[i].firstedge=s;     //将新结点*S插入顶点Vi的边表头部

		s=(EdgeNode *)malloc(sizeof(EdgeNode)); 

		s->adjvex=i;                  //邻接点序号为i

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

typedef EdgeNode*  TYPE;
EdgeNode*  open[MaxVertexNum]; //open表，next指向父节点
EdgeNode*  close[MaxVertexNum];//close表，next指向父节点，下标表示编号
int visit[MaxVertexNum] = {FALSE};

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
//出队列
//参数说明：open 数组，in 队首指针，out 队尾指针
int GetQueue(TYPE *open,int *in,int *out,TYPE *value){
	if((*in) == (*out)){		//队空条件 in == out
		return ERROR;
	}
	*value = open[*out];
//	open[*out] = NULL;				//出队，置位
	(*out) = ((*out)+1)%MaxVertexNum;
	return SUCCESS;
}

//宽度优先搜索算法

int  BFS(ALGraph *G,char value){
	//memset(open,NULL,sizeof(MaxVertexNum));//初始化open和close表
	//memset(close,NULL,sizeof(MaxVertexNum));
	int i,n;
	int inOpen,outOpen,emptyOpen;
	int inClose,outClose,emptyClose;
	n = 0;
	memset(visit,0,sizeof(visit));
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	emptyOpen = emptyClose = 0;
	EdgeNode  *tempNode;			//临时节点 n
	tempNode = G->adjlist[0].firstedge;
	visit[0] = TRUE;
	EnQueue(open,&inOpen,&outOpen,tempNode);//将第一个节点的下一个节点存入open表
	while(1){
		emptyOpen = GetQueue(open,&inOpen,&outOpen,&tempNode);	//从open表中取出第一个值
		n = tempNode->adjvex;
		visit[n] = TRUE;
		printf("when BFS find the %d element : %c\n",n,G->adjlist[n].vertex);
		EnQueue(close,&inClose,&outClose,tempNode);		//将该节点存入close表
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			do{						//输出close表中所有的节点
				emptyClose = GetQueue(close,&inClose,&outClose,&tempNode);
				if(emptyClose != -1){
					n = tempNode->adjvex;
					printf("BFS close element : %c\n",G->adjlist[n].vertex);
				}
			}while(emptyClose != -1);
			printf("BFS fine %c. \n",value);
			return SUCCESS; 	
		}
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			printf("BFS not find %c. \n",value);
			return ERROR;
		}
		tempNode = G->adjlist[i].firstedge;			//找到后继节点
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(visit[n] == FALSE){
				EnQueue(open,&inOpen,&outOpen,tempNode);
				visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}
	}
}

//深度优先搜索

int DFS(ALGraph *G,char value){
	//memset(open,NULL,sizeof(MaxVertexNum));//初始化open和close表
	//memset(close,NULL,sizeof(MaxVertexNum));
	memset(visit,0,sizeof(visit));
	int i,n;
	int inOpen,emptyOpen;
	int inClose,emptyClose;
	n = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	emptyOpen = emptyClose = 0;
	EdgeNode  *tempNode;			//临时节点 n
	tempNode = G->adjlist[0].firstedge;
	visit[0] = TRUE;
	Push(open,&inOpen,tempNode);//将第一个节点的下一个节点存入open表
	while(1){
		emptyOpen = Pop(open,&inOpen,&tempNode);	//从open表中取出第一个值
		n = tempNode->adjvex;
		visit[n] = TRUE;
		printf("when DFS find the %d element : %c\n",n,G->adjlist[n].vertex);
		Push(close,&inClose,tempNode);		//将该节点存入close表
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			do{						//输出close表中所有的节点
				emptyClose = Pop(close,&inClose,&tempNode);
				if(emptyClose != -1){
					n = tempNode->adjvex;
					printf("DFS close element : %c\n",G->adjlist[n].vertex);
				}
			}while(emptyClose != -1);
			printf("DFS fine %c. \n",value);
			return SUCCESS; 	
		}
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			printf("DFS not find %c. \n",value);
			return ERROR;
		}
		tempNode = G->adjlist[n].firstedge;			//找到后继节点
		while(tempNode != NULL){
			n = tempNode->adjvex;
			if(visit[n] == FALSE){
				Push(open,&inOpen,tempNode);
				visit[n] = TRUE;
			}
			tempNode = tempNode->next;
		}	
	}
}
/*
//有界深度优先搜索算法
//问题：如何表示深度
int  LimitDFS(ALGraph *G,char value){
	//memset(open,NULL,sizeof(MaxVertexNum));//初始化open和close表
	//memset(close,NULL,sizeof(MaxVertexNum));
	int i,n;
	int inOpen,outOpen,emptyOpen;
	int inClose,outClose,emptyClose;
	n = 0;
	inOpen = inClose = 0;			//初始化队列的队首和队尾
	outOpen = outClose = 0;
	emptyOpen = emptyClose = 0;
	EdgeNode  *tempNode;			//临时节点 n
	tempNode = G->adjlist[0].firstedge;
	visit[0] = TRUE;
	Push(open,&inOpen,&outOpen,tempNode);//将第一个节点的下一个节点存入open表
	while(1){
		emptyOpen = Pop(open,&inOpen,&outOpen,&tempNode);	//从open表中取出第一个值
		n = tempNode->adjvex;
		visit[n] = TRUE;
		printf("when find the %d element : %c\n",n,G->adjlist[n].vertex);
		Push(close,&inClose,&outClose,tempNode);		//将该节点存入close表
		if(G->adjlist[n].vertex == value){			//是否为目标节点？
			do{						//输出close表中所有的节点
				emptyClose = Pop(close,&inClose,&outClose,&tempNode);
				if(emptyClose != -1){
					n = tempNode->adjvex;
					printf("close element : %c\n",G->adjlist[n].vertex);
				}
			}while(emptyClose != -1);
			printf("DFS fine %c. \n",value);
			return SUCCESS; 	
		}
		if(emptyOpen == -1){					//open表为空,找不到目标节点
			printf("DFS not find %c. \n",value);
			return ERROR;
		}
		for(i = 0;i < MaxVertexNum; i++){			//找到该节点的后续节点，并存入open 表的末端
			if(G->adjlist[i].vertex == G->adjlist[n].vertex){
				tempNode = G->adjlist[i].firstedge;
				while(tempNode != NULL){
					n = tempNode->adjvex;
					if(visit[n] == FALSE){
						Push(open,&inOpen,&outOpen,tempNode);
						visit[n] = TRUE;
					}
					tempNode = tempNode->next;
				}	
				break;
			}
		}
	}
}


*/
int main(){
	ALGraph G;
	CreatALGraph(&G);
	show(&G);
	DFS(&G,'c');
	BFS(&G,'c');
	return 0;
}
