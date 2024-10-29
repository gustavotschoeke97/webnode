#include "weblist_pri.h"

int int_pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

void libera_arvore(ArvoreNode *arvore_node) {
    if (arvore_node == NULL) return;

    if (arvore_node->eh_folha) {
        if (arvore_node->node) {
            if (arvore_node->node->lista != NULL) {
                dDDLL(&(arvore_node->node->lista));
            }
            free(arvore_node->node);
        }
    } else {
        for (int i = 0; i < MAXIMO_NOS; i++) {
            libera_arvore(arvore_node->folhas[i]);
        }
    }
    free(arvore_node);
}

ArvoreNode *criar_arvore(int nivel_atual, int nivel_maximo, int *contador_chave) {
    ArvoreNode *arvore_no = malloc(sizeof(ArvoreNode));
    if (arvore_no == NULL) return NULL;

    if (nivel_atual == nivel_maximo) {
        arvore_no->eh_folha = 1;
        
        arvore_no->node = malloc(sizeof(Node));
        if (arvore_no->node == NULL) {
            free(arvore_no);
            return NULL;
        }
        
        arvore_no->node->chave = (*contador_chave)++;
        arvore_no->node->lista = NULL;
    } else {
        arvore_no->eh_folha = 0;
        arvore_no->node = NULL;
        for (int i = 0; i < MAXIMO_NOS; i++) {
            arvore_no->folhas[i] = criar_arvore(nivel_atual + 1, nivel_maximo, contador_chave);
            if (arvore_no->folhas[i] == NULL) {
                for (int j = 0; j < i; j++) {
                    libera_arvore(arvore_no->folhas[j]);
                }
                free(arvore_no);
                return NULL;
            }
        }
    }
    return arvore_no;
}

int cWL(ppWL ppWL, int nivel, int sizedata) {
    if(ppWL == NULL || *ppWL == NULL || nivel < 0 || sizedata <= 0) return FALHOU;

    *ppWL = (pWL) malloc(sizeof(WebList));
    if (*ppWL == NULL) {
        return FALHOU;
    }

    (*ppWL)->sizedata = sizedata;
    (*ppWL)->nivel = nivel;

    int num_nos_folha =  int_pow(MAXIMO_NOS, nivel + 1);
    (*ppWL)->total_de_folhas = num_nos_folha;
    (*ppWL)->contagem_nos_folhas = (int*) calloc(num_nos_folha, sizeof(int));
    if ((*ppWL)->contagem_nos_folhas == NULL) {
        free(*ppWL);
        *ppWL = NULL;
        return FALHOU;
    }

    int contador_chave = 0;
    (*ppWL)->raiz = criar_arvore(0, nivel, &contador_chave);
    if ((*ppWL)->raiz == NULL) {
        free((*ppWL)->contagem_nos_folhas);
        free(*ppWL);
        *ppWL = NULL;
        return FALHOU;
    }

    return SUCESSO;
}

int dWL(ppWL ppWL) {
    if(ppWL == NULL || *ppWL == NULL) return FALHOU;

    libera_arvore((*ppWL)->raiz);
    free(*ppWL);
    *ppWL = NULL;

    return SUCESSO;
}