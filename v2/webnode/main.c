

#include "stdio.h"
#include "stdlib.h"
#include "weblist_pri.h"


void imprime_int(void *a) {
  int int_a = *((int *)a);
  printf("valor: %d \n", int_a);
}

int compare_int(void *a, void *b) {
     int int_a = *((int *)a);
  int int_b = *((int *)b);

  if (int_a > int_b) return 1;
  if (int_a < int_b) return -1;
  return 0;
}

void imprime_ddll(pDDLL pDDLL, int sizedata, void (*imprime)(void *a)) {
    int contagem = countElements(pDDLL);
    void *temp = malloc(sizeof(sizedata));

    if(temp != NULL) {
        for (int i = 0; i < contagem; i++) {
            if(sPosition(pDDLL, i, temp) == SUCCESS) {
                imprime(temp);
            }
        }

        free(temp);
    }
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

    int res = cWL(&webList, 1, sizeof(int));

    if(res != SUCCESS) {
        printf("Falha ao criar a weblist \n");
    }

    int lista_int[15] = {10, 20, 1, 5, 2, 3, 25, 40, 11, 12, 31, 12, 13, 14, 15};


    for(int i = 0; i < 15; i++) {
        if(iDado(webList, &lista_int[i], compare_int) != SUCCESS) {
                    printf("Erro ao inserir o dados \n");
        }
    }

    int r = 10;
    res = rDado(webList, &r, compare_int);
   if(res != SUCCESS) {
        printf("Falha ao remover \n");
    }

    pLista(webList, imprime_int);


    int s = 31;
    if(bDado(webList, &s, compare_int) == SUCCESS) {
        printf("Dado encontrado: %d \n", s);
    } else {
        printf("Nao encontrou");
    }

    ppDDLL cp = malloc(sizeof(pDDLL));
    if (cp == NULL) {
        printf("Erro ao alocar memória para a cópia da lista\n");
    } else if (cpLista(webList, 1, cp) == SUCCESS) {
        // Exibe os elementos da lista copiada
        printf("Conteúdo da lista copiada:\n");
        imprime_ddll(*cp, webList->sizedata, imprime_int);
    } else {
        printf("Erro ao copiar a lista\n");
        free(cp); 
    }

    dWL(&webList);
    if (cp && *cp != NULL) {
        dDDLL(cp);
    }
    free(cp); 
};


int main() {

    deve_criar_weblist();
    deve_inserir_um_dado_do_tipo_int();

    return 0;
}