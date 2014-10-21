/************************************************************************/
/* 图的邻接表存储结构                                                    */
/************************************************************************/
#include <stdio.h>
#define MaxVertexNum 100
#define QueueSize 30 
typedef enum{FALSE,TRUE}Boolean;   
Boolean visited[MaxVertexNum];  
typedef char VertexType;
typedef int EdgeType;
typedef struct node		//边表结点
{
	int adjvex;			//邻接点域
	struct node *next;	//域链
	//若是要表示边上的权,则应增加一个数据域
}EdgeNode;
typedef struct vnode	//顶点边结点
{
	VertexType vertex;	//顶点域
	EdgeNode *firstedge;//边表头指针
}VertexNode;
typedef VertexNode AdjList[MaxVertexNum];	//AdjList是邻接表类型
typedef struct 
{
	AdjList adjlist;	//邻接表
	int n,e;			//图中当前顶点数和边数
}ALGraph;				//对于简单的应用，无须定义此类型，可直接使用AdjList类型
/************************************************************************/
/* 建立无向图的邻接表算法                                               */
/************************************************************************/
void CreateGraphAL (ALGraph *G)   
{   
	int i,j,k;   
	EdgeNode * s;   
	printf("请输入顶点数和边数(输入格式为:顶点数,边数)：/n");   
	scanf("%d,%d",&(G->n),&(G->e));       // 读入顶点数和边数   
	printf("请输入顶点信息(输入格式为:顶点号<CR>)每个顶点以回车作为结束:/n");   
	for (i=0;i<G->n;i++)              // 立有n个顶点的顶点表   
	{    
		scanf("/n%c",&(G->adjlist[i].vertex)); // 读入顶点信息   
		G->adjlist[i].firstedge=NULL;            // 点的边表头指针设为空   
	}   
	printf("请输入边的信息(输入格式为:i,j)：/n");   
	for (k=0;k<G->e;k++)      // 建立边表   
	{    
		scanf("/n%d,%d",&i,&j); // 读入边<Vi,Vj>的顶点对应序号   
		s=new EdgeNode;         // 生成新边表结点s   
		s->adjvex=j;         // 邻接点序号为j   
		s->next=G->adjlist[i].firstedge; // 将新边表结点s插入到顶点Vi的边表头部   
		G->adjlist[i].firstedge=s;   
		s=new EdgeNode;
		s->adjvex=i;
		s->next=G->adjlist[j].firstedge;
		G->adjlist[j].firstedge=s;
	}   
}  
/************************************************************************/
/* 深度优先遍历                                                         */
/************************************************************************/ 
void DFS(ALGraph *G,int i)
{ 
	//以vi为出发点对邻接表表示的图G进行深度优先搜索
	EdgeNode *p;
	printf("visit vertex:%c/n",G->adjlist[i].vertex);  // 访问顶点vi
	visited[i]=TRUE;		    	//标记vi已访问
	p=G->adjlist[i].firstedge;	    //取vi边表的头指针
	while(p)
	{								//依次搜索vi的邻接点vj，这里j=p->adjvex
		if (!visited[p->adjvex])	//若vi尚未被访问
			DFS(G,p->adjvex);		//则以Vj为出发点向纵深搜索
		p=p->next;					 //找vi的下一邻接点
	}
}
void DFSTraverseM(ALGraph *G)   
{   
	int i;   
	for(i=0;i<G->n;i++)   
		visited[i]=FALSE;      
	for(i=0;i<G->n;i++)   
		if(!visited[i])    
			DFS(G,i);   
} 
/************************************************************************/
/* 广度优先遍历                                                         */
/************************************************************************/
typedef struct  
{   
	int front;   
	int rear;   
	int count;   
	int data[QueueSize];   
}CirQueue;   
void InitQueue(CirQueue *Q)   
{   
	Q->front=Q->rear=0;   
	Q->count=0;   
}   
int QueueEmpty(CirQueue *Q)   
{   
	return Q->count=QueueSize;   
}   
int QueueFull(CirQueue *Q)   
{   
	return Q->count==QueueSize;   
}   
void EnQueue(CirQueue *Q,int x)   
{    
	if (QueueFull(Q))   
		printf("Queue overflow");   
	else  
	{    
		Q->count++;   
		Q->data[Q->rear]=x;   
		Q->rear=(Q->rear+1)%QueueSize;   
	}   
}   
int DeQueue(CirQueue *Q)   
{   
	int temp;   
	if(QueueEmpty(Q))   
	{    
		printf("Queue underflow");   
		return -1;   
	}   
	else  
	{   
		temp=Q->data[Q->front];   
		Q->count--;   
		Q->front=(Q->front+1)%QueueSize;   
		return temp;   
	}   
}   
void BFS(ALGraph*G,int k)
{	// 以vk为源点对用邻接表表示的图G进行广度优先搜索
	int i;
	CirQueue Q;				//须将队列定义中DataType改为int
	EdgeNode *p;
	InitQueue(&Q);			//队列初始化
	printf("visit vertex:%c/n",G->adjlist[k].vertex);		//访问源点vk
	visited[k]=TRUE; 
	EnQueue(&Q,k);			//vk已访问，将其人队。（实际上是将其序号人队）
	while(!QueueEmpty(&Q))
	{									//队非空则执行
		i=DeQueue(&Q);					//相当于vi出队
		p=G->adjlist[i].firstedge;		//取vi的边表头指针
		while(p)
		{								//依次搜索vi的邻接点vj(令p->adjvex=j)
			if(!visited[p->adjvex])
			{							//若vj未访问过
				printf("visit vertex:%c",G->adjlist[p->adjvex].vertex);		//访问vj
				visited[p->adjvex]=TRUE; 
				EnQueue(&Q,p->adjvex);	//访问过的vj人队
			}
			p=p->next;					//找vi的下一邻接点
		}
	}
}
void BFSTraverseM(ALGraph *G)   
{   
	int i;   
	for (i=0;i<G->n;i++)   
		visited[i]=FALSE;   
	for (i=0;i<G->n;i++)   
		if (!visited[i])    
			BFS(G,i);   
}   
/************************************************************************/
/* 主函数调用                                                           */
/************************************************************************/
int main()
{
	ALGraph G;
	CreateGraphAL(&G);
	printf("深度优先遍历：/n");
	DFSTraverseM(&G);
	printf("广度优先遍历：/n");
	BFSTraverseM(&G);
	return 0;
}
