#include "main.h"

int compInt(void *p1,void *p2){
	int param1 = *((int *)p1);
	int param2 = *((int *)p2);
	
	if(param1 > param2){
		return 1;
	}else if(param1 < param2){
		return -1;
	}
	return 0;
}

int main(int argc, char *argv[]) {
	
	pweblist ptr = NULL;
	int ret = 0;
	
	//1 criar webnode
	if(cWL(&ptr,0,sizeof(int)) == SUCCESS){
		printf(" WebNode tree criada com sucesso.\n");
	}else{
		printf(" Erro na criação da Webnode.\n");
	}
	
	//2 Inserir dados na webnode
	int value;
	int arrTotalInsert = 10;
	int i,*arr;
	
	arr = malloc(sizeof(int)*arrTotalInsert);
	for(i = 0;i<arrTotalInsert;i++){
		value= i*10;
		if(iDado(ptr,&value,compInt) == SUCCESS){
			printf("valor adicionado %d \n",value);
		}else{
			printf("Erro ao inserir elemento \n");
		}
		arr[i] = value;
	}
	
	nroEleWL(ptr,&ret);
	printf("Quantidade de nohs %i \n",ret);
	
	
	if(bDado(ptr,&arr[3],compInt) == SUCCESS){
		printf("elemento %d encontrado \n",arr[3]);
	}
	
	free(arr);
	return 0;
	
	
}
