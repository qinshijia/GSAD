
/*����ʵ��ѭ������*/

#include<stdio.h>
#include<stdlib.h>

#define SUCCESS 0		//�����ɹ�
#define ERROR	-1		//����ʧ��
#define MaxSize	50		//������󳤶�

typedef	int	TYPE;		//��������

//�����
//����˵����open ���飬in ����ָ�룬out ��βָ�룬value ��ӵ�ֵ
int EnQueue(TYPE *open,int *in,int *out,TYPE value){
	int temp;
	temp = ((*in)+1)%MaxSize;		//�������� in+1 = out
	if(temp == (*out)){
		return ERROR;
	}
	open[*in] = value;
	(*in) = temp;
	return SUCCESS;
}
//������
//����˵����open ���飬in ����ָ�룬out ��βָ��
TYPE GetQueue(TYPE *open,int *in,int *out){
	TYPE value;
	if((*in) == (*out)){		//�ӿ����� in == out
		return ERROR;
	}
	value = open[*out];
	open[*out] = 0;				//���ӣ���λ
	(*out) = ((*out)+1)%MaxSize;
	return value;
}

int main(){
	TYPE open[MaxSize];
	int in,out;		//���� ����β
	int i;
	int value;
	value = 0;
	in = 0;
	out = 0;
	//memset(open,0,sizeof(open));
	for(i = 0;i < MaxSize; i++){
		open[i] = 0;
	}
	EnQueue(open,&in,&out,1);
	EnQueue(open,&in,&out,2);
	EnQueue(open,&in,&out,3);
	do{								//ȫ������
		value = GetQueue(open,&in,&out);
		if(value != -1){ 
			printf("EnQueue = %d\n",value);
		}
	}while(value != -1);
	printf("********************************\n");
	EnQueue(open,&in,&out,4);
	do{
		value = GetQueue(open,&in,&out);
		if(value != -1){ 
			printf("EnQueue = %d\n",value);
		}
	}while(value != -1);
	printf("********************************\n");
	EnQueue(open,&in,&out,2);
	EnQueue(open,&in,&out,3);

	for(i = 0;i < MaxSize; i++){
		printf("EnQueue[%d] = %d\n",i,open[i]);
	}
	system("pause");
}