#include "type.h"

extern EdgeNode*  open[MaxVertexNum]; //open表
extern EdgeNode*  Close[MaxVertexNum];//close表
extern ALGraph *G;
extern int Visit[MaxVertexNum];

void 		CreatALGraph(const char fileName[]);
Result	 	BFS(char start ,char target,int showFlag);
Result 		DFS(char start ,char target,int showFlag);
Result 		DLS(char start ,char target,int showFlag);
Result 		IDS(char start ,char target,int showFlag);
Result 		UCS(char start ,char target,int showFlag);
Result 	 	BestFS(char start ,char target,int showFlag);
void freeGraph();


