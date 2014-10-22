
/*
#define MaxVertexNum 	50        	//定义最大顶点数
#define SUCCESS		0		//函数执行成功
#define FAILURE 	-1		//函数执行失败
#define ERROR		-1		//函数执行失败
//#define TRUE 		1		//已经遍历过
//#define FALSE 		0		//未遍历
#define DEEPLIMIT 	2		//有界深度优先搜索算法的 深度界限
*/
#define MaxVertexNum 	50        	//定义最大顶点数
typedef struct node{       	//边表结点

	char adjvex;          	//邻接点域
	struct node *next,*father;    //链域
	int deep;		//节点深度
	int cost;		//代价
	int evaluate;		//估价
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

extern EdgeNode*  open[MaxVertexNum]; //open表，next指向父节点
extern EdgeNode*  Close[MaxVertexNum];//close表，next指向父节点，下标表示编号
extern int Visit[MaxVertexNum];

void 	CreatALGraph(ALGraph *G,char fileName[]);
int  	BFS(ALGraph *G,char value);
int 	DFS(ALGraph *G,char value);
int 	LimitDFS(ALGraph *G,char value);
int 	IterDFS(ALGraph *G,char value);
int  	CostSearch(ALGraph *G,char value);
int  	BestSearch(ALGraph *G,char value);
