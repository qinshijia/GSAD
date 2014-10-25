#include<stdio.h>
int main(){
	int open[10];
	int i,j,value;
	j = 0;
	value = 0;
	for(i = 0;i < 10; i++){
		open[i] = 0;
	}
	for(i = 0;i < 5; i++){
		open[i] = i+1;
	}
	for(i ; i > j; i--){
		if(value < open[i-1]){
			open[i] = open[i-1];
		}else{
			open[i] = value;
			break;
		}
	}
	if(i == j){
		open[i] = value;
	}
	
	for(i = 0;i < 10; i++){
		printf("%d\t",open[i]);
	}
}
