/*********************************************************
  *���Ա�Ĳ���
  *����ʱ��	2014.09.30 19��57
  *����		renxinyuan
  *����		renxinyuanweb@163.com
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

#define SUCCESS	0			/*���سɹ�*/
#define	ERROR	-1			/*����ʧ��*/

typedef	int TYPE ;			/*ָ��Ԫ�ص�����*/

/********************************************************
  *
  *����ڵ�
  *
  *******************************************************/

struct node{
	TYPE	data;
	struct node	*prior,*next;
};

/*******************************************************
  *��ʾ����
  *���룺L ���Ա�ͷָ��
  *���ã��������������������Ԫ�ص�ֵ
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
  *��ʼ�����Ա�
  *���룺Ҫ�������Ա��ͷָ��
  *���أ�0 �ɹ���-1 ʧ��
  *�������������һ���յ����Ա�
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
  *�������Ա�
  *���룺Ҫ�������Ա��ͷָ��
  *���أ�0 �ɹ���-1 ʧ��
  *����������������Ա�
  ****************************************************/
void ListDestory(struct node **L){
	struct node * temp = NULL;
	if(*L != NULL){ //L!=NULL���ͷ�
		do{
			temp = (*L)->next;
			free(*L);
			*L = temp;
		}while(*L != NULL);
	}
}

/*****************************************************
  *������Ա�
  *���룺Ҫ������Ա��ͷָ��
  *���أ�0 �ɹ���-1 ʧ��
  *������������Ա�����
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
  *�ж��Ƿ�Ϊ��
  *���룺���Ա�ͷָ��
  *���أ�0 Ϊ�գ�-1 ��Ϊ��
  *��������������Ա���Ӱ��
  *****************************************************/
int ListEmpty(struct node *L){
	if(L->next != NULL)
		return ERROR;
	else 
		return SUCCESS;
}

/*****************************************************
  *�����Ա�ĳ���
  *���룺���Ա�ͷָ��
  *���أ� -1 ���󣬴���-1������ ���Ա�ĳ���
  *��������������Ա���Ӱ��	
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
  *��ȡ��i��Ԫ��
  *���룺 L ���Ա�ͷָ�룬i ��i��Ԫ�أ�e ��ȡԪ�صĴ洢λ��
  *���أ�0 �ɹ���-1 ʧ��
  *��������������Ա���Ӱ��	
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
  *Ԫ�������Ա��е�λ��
  *���룺L ���Ա�ͷָ�룬e Ԫ��
  *���أ�-1 δ�ҵ������� 0 ������ Ԫ�������Ա��λ��
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
  *����Ԫ��
  *���룺L ���Ա�ͷָ�룬i ��i��Ԫ��֮ǰ���룬e ���������
  *���أ�0 �ɹ���-1 ʧ��
  *�����������L �е�i��λ��֮ǰ�����µ�����Ԫ�� e,L �ĳ��ȼ� 1
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
  *ɾ��Ԫ��
  *����:L ���Ա�ͷָ��,i ��i��Ԫ��
  *���أ�0 �ɹ���-1 ʧ��
  *�������: ɾ����i��Ԫ��
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
