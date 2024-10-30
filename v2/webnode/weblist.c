#include "weblist_pri.h"
#include <stdlib.h>

int int_pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

void libera_arvore(NoArvore *no_arvore) {
    if (no_arvore == NULL) return;

    if (no_arvore->ehFolha) {
        if (no_arvore->noFolha) {
            if (no_arvore->noFolha->ddll != NULL) {
                dDDLL(&(no_arvore->noFolha->ddll));
            }
            free(no_arvore->noFolha);
        }
    } else {
        for (int i = 0; i < MAXIMO_NOS; i++) {
            libera_arvore(no_arvore->filhos[i]);
        }
    }
    free(no_arvore);
}

NoArvore *criarNoArvore(int nivel_atual, int nivel_maximo, int *contador_chave, NoArvore **nos_arvore_array, int *indice_folha) {
    NoArvore* no_arvore = (NoArvore*)malloc(sizeof(NoArvore));

    if(no_arvore == NULL) return NULL;

    if(nivel_atual == nivel_maximo) {
        // Nó folha
        no_arvore->ehFolha = 1;
        no_arvore->noFolha = (NoWebList*)malloc(sizeof(NoWebList));

        no_arvore->noFolha->chave = (*contador_chave)++; // chave incremental
        no_arvore->noFolha->ddll = NULL; // inicializa ddll

        // Armazena o ponteiro em um array para acesso rápido
        nos_arvore_array[*indice_folha] = no_arvore;
        (*indice_folha)++; // Incrementa o índice

        // Inicializa os filhos como NULL
        for (int i = 0; i < MAXIMO_NOS; i++) {
            no_arvore->filhos[i] = NULL;
        }
    } else {
        // Nó da árvore - interno
        no_arvore->ehFolha = 0;
        no_arvore->noFolha = NULL;

        for (int i = 0; i < MAXIMO_NOS; i++) {
            // criar recursivamente os demais filhos
            no_arvore->filhos[i] = criarNoArvore(nivel_atual + 1, nivel_maximo, contador_chave, nos_arvore_array, indice_folha);

            // libera árvore caso dê erro na alocação de memória
            if(no_arvore->filhos[i] == NULL) {
                for (int j = 0; j < i; j++) {
                    libera_arvore(no_arvore->filhos[j]);    
                }
                free(no_arvore);
                return NULL;
            }
        }
    }

    return no_arvore;
}

int cWL(ppweblist ppWL, int nivel, int sizedata) {
    if(ppWL == NULL || nivel < 0 || sizedata <= 0) return FAIL;

    *ppWL = (pweblist) malloc(sizeof(WebList));
    if(*ppWL == NULL) return FAIL;

    (*ppWL)->nivel = nivel;
    (*ppWL)->sizedata = sizedata;

    int total_folhas = int_pow(MAXIMO_NOS, nivel);
    (*ppWL)->totalFolhas = total_folhas;

    // Array com índices das folhas
    (*ppWL)->ponteirosNoArvore = (NoArvore **) malloc(total_folhas * sizeof(NoArvore*));
    if((*ppWL)->ponteirosNoArvore == NULL) {
        free(*ppWL);
        return FAIL;
    }

    // Cria um array zerado [0, 0, 0 ...]
    (*ppWL)->contagemFolhas = (int*)calloc(total_folhas, sizeof(int));
    if((*ppWL)->contagemFolhas == NULL) {
        free((*ppWL)->ponteirosNoArvore);
        free(*ppWL);
        return FAIL;
    }

    int indice_folha = 0;
    int contador_chave = 0;

    (*ppWL)->raiz = criarNoArvore(0, nivel, &contador_chave, (*ppWL)->ponteirosNoArvore, &indice_folha);
    if((*ppWL)->raiz == NULL) {
        free((*ppWL)->contagemFolhas);
        free((*ppWL)->ponteirosNoArvore);
        free(*ppWL);
        return FAIL;
    }

    return SUCCESS;
}

int dWL(ppweblist ppWL) {
    if(ppWL == NULL || *ppWL == NULL) return FAIL;

    libera_arvore((*ppWL)->raiz);
    free((*ppWL)->contagemFolhas);
    free((*ppWL)->ponteirosNoArvore);
    free(*ppWL);
    *ppWL = NULL;

    return SUCCESS;
}

int iDado(pweblist pWL, void* dado) {
    if(pWL == NULL || dado == NULL) return FAIL;

    int min_contagem = pWL->contagemFolhas[0];
    for(int i = 0; i < pWL->totalFolhas; i++) {
        if(pWL->contagemFolhas[i] < min_contagem) {
            min_contagem = pWL->contagemFolhas[i];
        }
    }

    int indice_folha = -1;
    for (int i = 0; i < pWL->totalFolhas; i++) {
        if(pWL->contagemFolhas[i] == min_contagem) {
            indice_folha = i;
            break;
        }
    }

    if(indice_folha == -1) return FAIL;

    NoArvore *no_arvore = pWL->ponteirosNoArvore[indice_folha];
    if(no_arvore == NULL || no_arvore->ehFolha != 1 || no_arvore->noFolha == NULL) {
        return FAIL;
    }

    if(no_arvore->noFolha->ddll == NULL) {
        int res = cDDLL(&(no_arvore->noFolha->ddll), pWL->sizedata);
        if(res != SUCCESS) return FAIL;
    }

    int res = iEnd(no_arvore->noFolha->ddll, dado);
    if(res != SUCCESS) return FAIL;

    // incrementa contagem folhas
    pWL->contagemFolhas[indice_folha]++;

    return SUCCESS;
}

// Função para percorrer a lista de dados
int pLista(pweblist pWL, void (*imprime)(void *a)) {

    if(pWL == NULL || imprime == NULL) return FAIL;

    for (int i = 0; i < pWL->totalFolhas; i++) {
        if(pWL->ponteirosNoArvore[i]->noFolha->ddll != NULL) {
            int contagem = countElements(pWL->ponteirosNoArvore[i]->noFolha->ddll);

            void *temp = malloc(pWL->sizedata);
            if(temp == NULL) return FAIL;

            for (int j = 0; j < contagem; j++) {
                if(sPosition(pWL->ponteirosNoArvore[i]->noFolha->ddll, j, temp) == SUCCESS) {
                    imprime(temp);
                }
            }

            free(temp);
        }
    }

    return SUCCESS;
}