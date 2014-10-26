#include "type.h"

extern EdgeNode*  open[MaxVertexNum]; //open表
extern EdgeNode*  Close[MaxVertexNum];//close表
extern ALGraph *G;
extern int Visit[MaxVertexNum];

void 		CreatALGraph(const char fileName[]);
Result	 	BFS(char start ,char target,int showFlag);
Result 		DFS(char start ,char target,int showFlag);
Result 		LimitDFS(char start ,char target,int showFlag);
Result 		IterDFS(char start ,char target,int showFlag);
Result 		CostSearch(char start ,char target,int showFlag);
Result 	 	BestSearch(char start ,char target,int showFlag);


