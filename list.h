int ListShow(struct node *L);
int ListInit(struct node **L);

void ListDestory(struct node **L);
int ListClear(struct node *L);
int ListEmpty(struct node *L);
int ListLength(struct node *L);
int ListGetElem(struct node *L,int i,TYPE *e);
int ListLocateElem(struct node *L,TYPE e);
int ListInsert(struct node *L,int i,TYPE e);
int ListDelete(struct node *L,int i);
