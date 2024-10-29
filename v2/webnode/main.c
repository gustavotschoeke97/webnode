

#include "stdio.h"
#include "stdlib.h"
#include "weblist_pri.h"


void deve_criar_weblist() {
    pWL webList = NULL;

    if(cWL(&webList, 2, sizeof(int) == SUCCESS)) {
        printf("Criou com sucesso a weblist");
    } else {
        printf("Erro ao criar a weblist");
    }

    dWL(&webList);
};


int main() {

    deve_criar_weblist();



    return 0;
}