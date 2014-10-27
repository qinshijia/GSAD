
#define MaxVertexNum 	50        	//定义最大顶点数
typedef struct node{       	//边表结点

	int  adjvex;          	//邻接点域
	struct node *next,*father;    //链域
	int deep;		//节点深度
	int cost;		//代价
	int evaluate;		//估价
}EdgeNode;

typedef struct vnode{     //顶点表结点
	char vertex;           //顶点域
	EdgeNode *firstedge; //边表头指针
}VertexNode;

typedef struct result{
	int numOpen;
	int numClose;
	int findFlag;
	int cost;
}Result;
typedef VertexNode AdjList[MaxVertexNum];       //AdjList是邻接表类型

typedef struct { 
	AdjList adjlist;     //邻接表
	int n,e;             //图中当前顶点数和边数
}ALGraph;               //图类型


