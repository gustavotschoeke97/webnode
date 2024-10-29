#include "ddll/DDLL_pub.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>
#include <string.h>
#include "weblist_pub.h"

#define MAXIMO_NOS 8
#define FALHOU 1
#define SUCESSO 0

typedef struct Node {
    int eh_folha;
    int chave;
    pDDLL lista;
} Node;

typedef struct ArvoreNode {
    int eh_folha;
    struct ArvoreNode *folhas[MAXIMO_NOS];
    Node *node;
} ArvoreNode;

typedef struct WebList {
    int nivel;
    ArvoreNode *raiz;
    int total_de_folhas;
    int *contagem_nos_folhas;
    int sizedata;
} WebList;