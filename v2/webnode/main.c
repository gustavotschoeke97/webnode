

#include "stdio.h"
#include "stdlib.h"
#include "weblist_pri.h"


void imprime_int(void *a) {
  int int_a = *((int *)a);
  printf("valor: %d \n", int_a);
}

void deve_criar_weblist() {
    pweblist webList = NULL;

    if(cWL(&webList, 2, sizeof(int) == SUCCESS)) {
        printf("Criou com sucesso a weblist \n");
    } else {
        printf("Erro ao criar a weblist \n");
    }

    dWL(&webList);
};

void deve_inserir_um_dado_do_tipo_int() {
    pweblist webList = NULL;

    int res = cWL(&webList, 2, sizeof(int));

    if(res != SUCCESS) {
        printf("Falha ao criar a weblist \n");
    }

    int v = 10;
    iDado(webList, &v);
    v = 12;
    iDado(webList, &v);
    v = 9;
    if(iDado(webList, &v) == SUCCESS) {
        pLista(webList, imprime_int);
        printf("Dado inserido com sucesso \n");
    } else {
        printf("Erro ao inserir o dado \n");
    }

    dWL(&webList);
};


int main() {

    deve_criar_weblist();
    deve_inserir_um_dado_do_tipo_int();

    return 0;
}