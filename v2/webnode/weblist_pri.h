#include "ddll/DDLL_pub.h"
#include "stdio.h"
#include "stdlib.h"
#include <math.h>
#include <string.h>
#include "weblist_pub.h"

#define MAXIMO_NOS 8

typedef struct NoWebList {
    int chave;                // Chave associada ao nó folha
    pDDLL ddll;               // Ponteiro para a DDLL
} NoWebList;

typedef struct NoArvore {
    int ehFolha;               // Indica se é nó folha
    struct NoArvore* filhos[MAXIMO_NOS]; // Ponteiros para os nós filhos
    NoWebList* noFolha;        // Ponteiro para o NoWebList se for folha
} NoArvore;

typedef struct weblist {
    int nivel;               // Nível da árvore
    int sizedata;            // Tamanho do tipo de dado armazenado
    NoArvore* raiz;          // Ponteiro para o nó raiz
    int totalFolhas;         // Total de nós folhas
    int *contagemFolhas;     // Contagem de nós folhas 
    NoArvore** ponteirosNoArvore; // Array de ponteiros para nós folha para acesso rápido
} WebList;