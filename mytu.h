#include "type.h"

extern EdgeNode*  open[MaxVertexNum]; //open表
extern EdgeNode*  Close[MaxVertexNum];//close表
extern int Visit[MaxVertexNum];

void 	CreatALGraph(ALGraph *G,char fileName[]);
int  	BFS(ALGraph *G,char value);
int 	DFS(ALGraph *G,char value);
int 	LimitDFS(ALGraph *G,char value);
int 	IterDFS(ALGraph *G,char value);
int  	CostSearch(ALGraph *G,char value);
int  	BestSearch(ALGraph *G,char value);


