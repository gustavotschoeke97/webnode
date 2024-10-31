#include "weblist_pri.h"
#include <stdio.h>
#include <stdlib.h>

int int_pow(int base, int exp) {
    int result = 1;
    for (int i = 0; i < exp; i++) {
        result *= base;
    }
    return result;
}

void liberaArvore(NoArvore *no_arvore) {
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
            liberaArvore(no_arvore->filhos[i]);
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
                    liberaArvore(no_arvore->filhos[j]);    
                }
                free(no_arvore);
                return NULL;
            }
        }
    }

    return no_arvore;
}


// Busca dado normal
int buscarEmDDLL(pDDLL ddll, void *dado, int sizedata) {
    if(ddll == NULL) return FAIL;

    int contagem = countElements(ddll);
    void *temp = malloc(sizedata);
    if( temp == NULL) return FAIL;

    for (int i = 0; i < contagem; i++) {
        if (sPosition(ddll, i, temp) == SUCCESS) {
            if(memcmp(temp, dado, sizedata) == 0) {
                free(temp);
                return SUCCESS;
            }
        }
    }

    free(temp);
    return FAIL;
}

int buscaBinariaEmDDLL(pDDLL ddll, void *dado, int sizedata, int (*compare)(void *a, void* b), int *posicao) {
    if(ddll == NULL) return FAIL;

    *posicao = -1;
    int inicio = 0;
    int fim = countElements(ddll) -1;
    void *temp = malloc(sizedata);

    if(temp == NULL) return FAIL;

  while (inicio <= fim) {
        int meio = (inicio + fim) / 2;

        if (sPosition(ddll, meio, temp) != SUCCESS) {
            free(temp);
            return FAIL;
        }

        int cmp = compare(temp, dado);
        if (cmp == 0) {
            *posicao = meio; // Armazena a posição do elemento encontrado
            free(temp);
            return SUCCESS;
        } else if (cmp < 0) {
            inicio = meio + 1;
        } else {
            fim = meio - 1;
        }
    }

    free(temp);
    return  FAIL;
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

    liberaArvore((*ppWL)->raiz);
    free((*ppWL)->contagemFolhas);
    free((*ppWL)->ponteirosNoArvore);
    free(*ppWL);
    *ppWL = NULL;

    return SUCCESS;
}

int iDado(pweblist pWL, void* dado, int (*cmp)(void *a , void *b)) {
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

    int pos_inserir = 0;
    int contagem = countElements(no_arvore->noFolha->ddll);
    void *temp = malloc(pWL->sizedata);
    if(temp == NULL) return FAIL;

    // encontra a posição correta para inserir
    for (int i = 0; i < contagem; i++) {
        if(sPosition(no_arvore->noFolha->ddll, i, temp) != SUCCESS) {
            free(temp);
            return FAIL;
        }

        if(cmp(dado, temp) < 0) {
            pos_inserir = i;
            break;
        }

        pos_inserir = i + 1; // insere no final caso nao encontre a posição
    }

    free(temp);

    int res = iPosition(no_arvore->noFolha->ddll, pos_inserir, dado);
    if(res != SUCCESS) return FAIL;

    // printf("Total: %d indice: %d \n", ++pWL->contagemFolhas[indice_folha], indice_folha);
    pWL->contagemFolhas[indice_folha]++;

    return SUCCESS;
}

// Função para remover um dado
int rDado(pweblist pWL, void* dado, int (*cmp) (void *a, void *b)) {
    if(pWL == NULL || dado == NULL || cmp == NULL) return FAIL;

    int removido = FAIL;
    int posicao;

    for (int i = 0; i < pWL->totalFolhas; i++) {
            if(pWL->ponteirosNoArvore[i]->noFolha->ddll != NULL) {
            if(buscaBinariaEmDDLL(pWL->ponteirosNoArvore[i]->noFolha->ddll, dado, pWL->sizedata, cmp, &posicao) == SUCCESS) {
                if(rPosition(pWL->ponteirosNoArvore[i]->noFolha->ddll, posicao, dado) == SUCCESS) {
                   if (pWL->contagemFolhas[i] > 0) {
                        pWL->contagemFolhas[i]--;
                    }

                    return SUCCESS;
                }
            }
        }
    }

    return removido;
}

int bDado(pweblist pWL, void* dado, int (*cmp)(void *a, void *b)) {
    if(pWL == NULL || dado == NULL || cmp == NULL) return FAIL;
    
    int encontrado = FAIL;
    int posicao;
    for(int i = 0; i < pWL->totalFolhas; i++) {
        if(pWL->ponteirosNoArvore[i]->noFolha->ddll != NULL) {
            if(buscaBinariaEmDDLL(pWL->ponteirosNoArvore[i]->noFolha->ddll, dado, pWL->sizedata, cmp, &posicao) == SUCCESS) {
                encontrado = SUCCESS;
                break;
            }
        }
    }

    return encontrado;
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


NoWebList *encontraNoPorChave(pweblist pWL, int chave) {
    if (pWL == NULL || chave < 0) return NULL;
    
    for (int i = 0; i < pWL->totalFolhas; i++) {
        if (pWL->ponteirosNoArvore[i]->noFolha->chave == chave) {
            return pWL->ponteirosNoArvore[i]->noFolha;
        }
    }

    return NULL;
}

int cpLista(pweblist pWL, int chave, ppDDLL retorno) {
    if (pWL == NULL || chave < 0 || retorno == NULL) return FAIL;

    NoWebList *noFolha = encontraNoPorChave(pWL, chave);
    if (noFolha == NULL || noFolha->ddll == NULL) return FAIL;

    if (cDDLL(retorno, pWL->sizedata) != SUCCESS) {
        return FAIL;
    }

    int contagem = countElements(noFolha->ddll);
    void *temp = malloc(pWL->sizedata);
    if (temp == NULL) {
        dDDLL(retorno);
        return FAIL;
    }

    for (int i = 0; i < contagem; i++) {
        if (sPosition(noFolha->ddll, i, temp) == SUCCESS) {
            if (iEnd(*retorno, temp) != SUCCESS) {
                free(temp);
                dDDLL(retorno);
                return FAIL;
            }
        }
    }

    free(temp);
    return SUCCESS;
}

int sbLista(pweblist pWL, int chave, pDDLL novaLista) {
    if(pWL == NULL || chave < 0 || novaLista == NULL) return FAIL;

    NoWebList *noFolha = encontraNoPorChave(pWL, chave);

    if(noFolha == NULL || noFolha->ddll == NULL) return FAIL;

    if(dDDLL(&(noFolha->ddll)) != SUCCESS) {
        return FAIL;
    }

    noFolha->ddll = novaLista;

    return SUCCESS;
}