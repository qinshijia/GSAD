#include "type.h"

extern EdgeNode*  open[MaxVertexNum]; //open表
extern EdgeNode*  Close[MaxVertexNum];//close表
extern ALGraph *G;
extern int Visit[MaxVertexNum];
typedef EdgeNode*  TYPE;

Result	 	BFS(char start ,char target,int showFlag);
Result 		DFS(char start ,char target,int showFlag);
Result 		DLS(char start ,char target,int showFlag);
Result 		IDS(char start ,char target,int showFlag);
Result 		UCS(char start ,char target,int showFlag);
Result 	 	BestFS(char start ,char target,int showFlag);
int CreatALGraph(const char fileName[]);
int valueIndex(char value);
void freeGraph();
void CreateFile();

int Pop(TYPE *open,int *out,TYPE *value,int showFlag);
int Push(TYPE *open,int *in,TYPE value,int showFlag);
int EnQueue(TYPE *open,int *in,int *out,TYPE value,int showFlag);
int OrderEnQueue(TYPE *open,int *in,int *out,TYPE value,int mode,int showFlag);
int GetQueue(TYPE *open,int *in,int *out,TYPE *value,int showFlag);

