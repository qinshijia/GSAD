#include<stdio.h>
#include<stdlib.h>

int main(){
	FILE *fp;
	if((fp = fopen("a.txt","r"))== NULL){
		perror("open file error\n");
	}
	int n,e;
//	e = fgetc(fp);
//	n = fgetc(fp);
	fscanf(fp,"%d%d",&e,&n);
	printf("e  = %d,n = %d\n",e,n);
	return 0;
}
