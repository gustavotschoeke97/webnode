#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "ddll/DDLL_pub.h"

// #define SUCCESS 0
// #define FAIL -1
#define MAXIMO_NOS 8

typedef struct NoWebList {
    int chave; // chave unica para o nó folha
    struct DDLL *lista;
} NoWebList;

typedef struct NoArvoreWebList {
    int eh_folha;
    struct NoArvoreWebList *filhos[MAXIMO_NOS];
    NoWebList *no_weblist;
} NoArvoreWebList;

typedef struct WebList {
    int nivel; // Nivel da Weblist
    NoArvoreWebList *raiz; // Nó raiz da weblist
    int num_nos_folha; // total de nós folhas
    NoArvoreWebList **nos_folha;  // array de ponteiros para nós folha para acesso rapido
    int *contagem_nos_folha; // array para contar dados em cada nó folha
    int sizedata; // tamanho do dado
} WebList, *pWebList, **ppWebList;