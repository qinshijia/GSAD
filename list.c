/*********************************************************
  *线性表的操作
  *创建时间	2014.09.30 19：57
  *作者		renxinyuan
  *邮箱		renxinyuanweb@163.com
  ********************************************************/
/*********************************************************
  include 
  *ListInit(&L)
  *ListDestroy(&L)
  *ListClear(&l)
  *ListEmpty(L)
  *ListLength(L)
  *ListGetElem(L,i,&e)
  *ListLocateElem(L,e,compare())
  *ListInsert(&L,i,e)
  *ListDelete(&L,i,&e)
  ********************************************************/

#include<stdio.h>
#include<stdlib.h>
#include<malloc.h>

#define SUCCESS	0			/*返回成功*/
#define	ERROR	-1			/*返回失败*/

typedef	int TYPE ;			/*指定元素的类型*/

/********************************************************
  *
  *定义节点
  *
  *******************************************************/

struct node{
	TYPE	data;
	struct node	*prior,*next;
};

/*******************************************************
  *显示函数
  *输入：L 线性表头指针
  *作用：依次输出表中所有数据元素的值
  *****************************************************/
int ListShow(struct node *L){
	struct node *showNode;
	int inode = 1;
	showNode = L;
	printf("\n**********show list**************\n");
	while(showNode->next != NULL){
		showNode = showNode->next;
		printf("*\tNo %d\t Data %d\t*\n",inode++,showNode->data);
	}
	printf("*********************************\n\n");
	return SUCCESS;
}

/*******************************************************
  *初始化线性表
  *输入：要建立线性表的头指针
  *返回：0 成功，-1 失败
  *操作结果：构造一个空的线性表
  ******************************************************/
int ListInit(struct node **L){
	struct node *head;
	head =(struct node *)malloc(sizeof(struct node));
	if(head == NULL)
		return ERROR;
	head->next = NULL;
//	if(head->next == NULL)
//		printf("head lllll is NULL");
	*L = head;
//	if(L->next == NULL)
//		printf("head wwwwwwww is NULL");
	return SUCCESS;
}


/*****************************************************
  *销毁线性表
  *输入：要销毁线性表的头指针
  *返回：0 成功，-1 失败
  *操作结果：销毁线性表
  ****************************************************/
void ListDestory(struct node **L){
	struct node * temp = NULL;
	if(*L != NULL){ //L!=NULL才释放
		do{
			temp = (*L)->next;
			free(*L);
			*L = temp;
		}while(*L != NULL);
	}
}

/*****************************************************
  *清空线性表
  *输入：要清空线性表的头指针
  *返回：0 成功，-1 失败
  *操作结果：线性表被重置
  ****************************************************/
int ListClear(struct node *L){
	struct node *clear,*clearHead;
	if(L->next != NULL){
		clear = L->next;
		while(clear->next != NULL){
			clearHead = clear->next;
			free(clear);
			if(clear != NULL)
				return ERROR;
			clear = clearHead;
		}
	}
	return SUCCESS;
}

/******************************************************
  *判定是否为空
  *输入：线性表头指针
  *返回：0 为空，-1 不为空
  *操作结果：对线性表无影响
  *****************************************************/
int ListEmpty(struct node *L){
	if(L->next != NULL)
		return ERROR;
	else 
		return SUCCESS;
}

/*****************************************************
  *求线性表的长度
  *输入：线性表头指针
  *返回： -1 错误，大于-1的整数 线性表的长度
  *操作结果：对线性表无影响	
  ****************************************************/
int ListLength(struct node *L){
	struct node *lenNode;
	int length;
	length = 0;
	if(L->next == NULL)
		return 0;
	else{
		lenNode = L;
		while(lenNode->next != NULL){
			lenNode =lenNode->next;
			length ++;			
		}

	}
	return length;
}

/****************************************************
  *获取第i个元素
  *输入： L 线性表头指针，i 第i个元素，e 获取元素的存储位置
  *返回：0 成功，-1 失败
  *操作结果：对线性表无影响	
  *********************************************************/
  
int ListGetElem(struct node *L,int i,TYPE *e){
	struct node *getnode;
	int inode;
	inode = 0;
	getnode = L;
	if(getnode->next == NULL || i <0)
		return ERROR;
	else{
		for(inode = 0;inode < i; inode++){
			getnode = getnode->next;
		}
		*e = getnode->data;
		return SUCCESS;
	}
}

/************************************************************
  *元素在线性表中的位置
  *输入：L 线性表头指针，e 元素
  *返回：-1 未找到，大于 0 的整数 元素在线性表的位置
  ***********************************************************/
int ListLocateElem(struct node *L,TYPE e){
	struct node *LocateNode;
	int inode;
	inode = 0;
	LocateNode = L;
	if(LocateNode == NULL)
		return ERROR;
	while(LocateNode->next != NULL){
		LocateNode = LocateNode->next;
		inode++;
		if(LocateNode->data == e){
			return inode;
		}
	}
	return ERROR;
}

/*******************************************************************
  *插入元素
  *输入：L 线性表头指针，i 在i个元素之前插入，e 插入的数据
  *返回：0 成功，-1 失败
  *操作结果：在L 中第i个位置之前插入新的数据元素 e,L 的长度加 1
  ****************************************************************/
int ListInsert(struct node *L,int i,TYPE e){
	struct node *insertNode,*nextNode,*newNode;
	int nextFlag;
	int inode ;
	inode = 0 ;
	newNode = (struct node *)malloc(sizeof(struct node));
	newNode->data = e;
	insertNode = L;
	nextFlag = 0;
	for(inode = 1;inode < i; inode++){
		if(insertNode->next == NULL)
			return ERROR;
		insertNode = insertNode->next;

	}
	if(insertNode->next != NULL){
		nextFlag = 1;
		nextNode =  insertNode->next;
	}
	insertNode->next = newNode;
	if(nextFlag)
		newNode->next = nextNode;
	else
		newNode->next = NULL;
	return SUCCESS;
}

/**********************************************************************
  *删除元素
  *输入:L 线性表头指针,i 第i个元素
  *返回：0 成功，-1 失败
  *操作结果: 删除第i个元素
  ********************************************************************/
int ListDelete(struct node *L,int i){
	struct node *delNode,*nextNode;
	int inode;
	int nextFlag;
	inode = 0;
	nextFlag = 0;
	delNode = L;
	for(inode = 1;inode < i; inode++){
		delNode = delNode->next;	
	}
	if(delNode->next != NULL){
		nextFlag = 1;
		nextNode = delNode->next;
	}
	if(nextFlag){
		delNode->next = nextNode->next;
		free(nextNode);
	}
	else
		delNode->next = NULL;
	return SUCCESS;
}

/*
int main(){
	struct node *L;
	ListInit(&L);
	L->next = NULL;
	if(L->next == NULL)
		printf("head->next is NULL\n");
	ListInsert(L,1,11);
	ListInsert(L,2,12);
	ListInsert(L,3,13);
	ListShow(L);
	ListLength(L);
	printf("Length of List is %d\n",ListLength(L));
	printf("Element 12 locate in No %d\n",ListLocateElem(L,12));
	ListDelete(L,2);
	ListShow(L);
	ListDestory(&L);
	return 0;

}
*/
