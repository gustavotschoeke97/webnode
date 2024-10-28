

#include "stdlib.h"
#include "string.h"

#include "weblist_pri.h"

void liberar_no_arvore_weblist(NoArvoreWebList *no) {
    if(no == NULL) return;

    if(no->eh_folha) {
        if(no->no_weblist) {
            if(no->no_weblist->lista) {
                dDDLL(&(no->no_weblist->lista));
            }

            free(no->no_weblist);
        }
    } else {
        for(int i = 0; i < MAXIMO_NOS; i++) {
            liberar_no_arvore_weblist(no->filhos[i]);
        }
    }

    free(no);
}

NoArvoreWebList* criar_no_arvore_weblist(int nivel_atual, int nivel_maximo, int *contador_chave, NoArvoreWebList **nos_folha, int *indice_no_folha) {
    NoArvoreWebList *no = (NoArvoreWebList*) malloc(sizeof(NoArvoreWebList));

    if(no == NULL) return NULL;

    if(nivel_atual == nivel_maximo) {
        // Nó folha
        no->eh_folha = 1;
        no->no_weblist = (NoWebList*) malloc(sizeof(NoWebList));
        if(no->no_weblist == NULL) {
            free(no);
            return NULL;
        }

        no->no_weblist->chave = (*contador_chave)++;
        no->no_weblist->lista = NULL; // incia a lista vazia
        
        // Armazena um ponteiro para o no folha
        nos_folha[(*indice_no_folha)++] = no;

        // incia os nos folhas como null
        for(int i = 0; i < MAXIMO_NOS; i++) {
            no->filhos[i] = NULL;
        }

    } else {
        // Nó não eh folha
        no->eh_folha = 0;
        no->no_weblist = NULL;

        for(int i; i < MAXIMO_NOS; i++) {
            no->filhos[i] = criar_no_arvore_weblist(nivel_atual + 1, nivel_maximo, contador_chave, nos_folha, indice_no_folha);

            if(no->filhos[i] == NULL) {
                for(int j = 0; j < i; j++) {
                    liberar_no_arvore_weblist(no->filhos[j]);
                }

                free(no);
                return  NULL;
            }
        }
    }

    return no;
}


int cWL(ppWebList pWL, int nivel, int sizedata) {
    if(!pWL || nivel < 0 || sizedata <= 0) return FAIL;

    // aloca memoria para weblist
    *pWL = (pWebList)malloc(sizeof(WebList));

    if(*pWL == NULL) return FAIL;

    // define a quantidade de niveis da arvore
    (*pWL)->nivel = nivel;
    (*pWL)->sizedata = sizedata;

    // numero de nós folha = 8 ^ 2 = 68
    int num_nos_folha = (int)pow(MAXIMO_NOS, nivel + 1);
    (*pWL)->num_nos_folha = num_nos_folha;

    // 
    (*pWL)->nos_folha = (NoArvoreWebList **)malloc(sizeof(NoArvoreWebList*) * num_nos_folha);
    if((*pWL)->nos_folha == NULL) {
        free(*pWL);
        *pWL = NULL;
        return FAIL;
    }

    (*pWL)->contagem_nos_folha = (int*)calloc(num_nos_folha, sizeof(int));
    if((*pWL)->contagem_nos_folha == NULL) {
        free((*pWL)->nos_folha);
        free(*pWL);
        *pWL = NULL;

        return FAIL;
    }

    int contador_chave = 0;
    int indice_no_folha = 0;

    (*pWL)->raiz = criar_no_arvore_weblist(0, nivel, &contador_chave, (*pWL)->nos_folha, &indice_no_folha);
    if((*pWL)->raiz == NULL) {
        free((*pWL)->contagem_nos_folha);
        free((*pWL)->nos_folha);
        free(*pWL);
        *pWL = NULL;

        return FAIL;
    }

    return SUCCESS;
}


int iDado(pWebList pWL, void *dado) {
    if(pWL == NULL || dado == NULL) {
        return FAIL;
    }

    // Encontrar a contagem minima entre nos folhas
    int min_contagem = pWL->contagem_nos_folha[0];
    for(int i = 1; i < pWL->num_nos_folha; i++) {
        if(pWL->contagem_nos_folha[i] < min_contagem) {
            min_contagem = pWL->contagem_nos_folha[i];
        }
    }

    // Encontrar o primeiro nó folha com a contagem mínima
    int indice_folha = -1;
    for(int i = 0; i < pWL->num_nos_folha; i++) {
        if(pWL->contagem_nos_folha[i] == min_contagem) {
            indice_folha = i;
            break;
        }
    }


    if(indice_folha == -1) {
        return FAIL;
    }

    NoArvoreWebList *no_folha = pWL->nos_folha[indice_folha];
    if(no_folha == NULL || no_folha->eh_folha == 0 || no_folha->no_weblist == NULL) {
        return FAIL;
    }

    // Se a DDLL não foi criada ainda, crie-a
    if(no_folha->no_weblist->lista == NULL) {
        int res = cDDLL(&(no_folha->no_weblist->lista), pWL->sizedata);

        if(res != SUCCESS) {
            return FAIL;
        }
    }

    // Inserir dado na DDLL
    int res = iEnd(no_folha->no_weblist->lista, dado);
    if(res != SUCCESS) {
        return FAIL;
    }

    // Atualizar a contagem
    pWL->contagem_nos_folha[indice_folha]++;

    return SUCCESS;
}

// Retornar o numeor de elementos em um no folha específico
int nroEleNoFolha(pWebList pWL, int chave, int *retorno) {
    if(pWL == NULL || chave >= pWL->num_nos_folha || retorno == NULL) {
        return FAIL;
    }

    *retorno = pWL->contagem_nos_folha[chave];

    return SUCCESS;
}


// Retornar o numero total de nos folha
int nroNoFolha(pWebList pWL, int *retorno) {
    if(pWL == NULL || retorno == NULL) {
        return FAIL;
    }

    *retorno = pWL->num_nos_folha;

    return SUCCESS;
}

// Retornar o número total de elementos na WebList
int nroEleWL(pWebList pWL, int *retorno) {
    if(pWL == NULL || retorno == NULL) {
        return FAIL;
    }

    int total = 0;
    for(int i = 0; i < pWL->num_nos_folha; i++) {
        total += pWL->contagem_nos_folha[i];
    }

    *retorno = total;

    return SUCCESS;
}

// Retornar uma lista com todas as chaves da WebList
int lstChaves(pWebList pWL, ppDDLL retorno) {
    if(pWL == NULL || retorno == NULL) {
        return FAIL;
    }

    int res = cDDLL(retorno, sizeof(int));
    if(res != SUCCESS) {
        return FAIL;
    }

    for (int i = 0; i < pWL->num_nos_folha; i++) {
        int chave = pWL->nos_folha[i]->no_weblist->chave;
        res = iEnd(*retorno, &chave);

        if(res != SUCCESS) {
            dDDLL(retorno);
            return  FAIL;
        }
    }

    return SUCCESS;
}

// Verificar se a WebList está balanceada
int WLbalanceada(pWebList pWL) {
    if(pWL == NULL) return FAIL;

    int min_contagem = pWL->contagem_nos_folha[0];
    int max_contagem = pWL->contagem_nos_folha[0];

    for(int i = 0; i < pWL->num_nos_folha; i++) {
        if(pWL->contagem_nos_folha[i] < min_contagem) {
            min_contagem = pWL->contagem_nos_folha[i];
        }

        if(pWL->contagem_nos_folha[i] > max_contagem) {
            max_contagem = pWL->contagem_nos_folha[i];
        }
    }

    if(max_contagem - min_contagem <= 1) {
        return SUCCESS;
    } else {
        return FAIL;
    }
}

// Criar uma DDLL vazia para a chave recebida
int nvLista(pWebList pWL, int chave) {
    if(pWL == NULL || chave < 0 || chave >= pWL->num_nos_folha) {
        return FAIL;
    }

    NoArvoreWebList *no_folha = pWL->nos_folha[chave];

    if(no_folha == NULL || no_folha->eh_folha == 0 || no_folha->no_weblist == NULL) {
        return SUCCESS;
    }

    if(no_folha->no_weblist->lista != NULL) {
        dDDLL(&(no_folha->no_weblist->lista));
        pWL->contagem_nos_folha[chave] = 0;
    }

    int res = cDDLL(&(no_folha->no_weblist->lista), pWL->sizedata);
    if(res != SUCCESS) {
        return FAIL;
    }

    return SUCCESS;
}


// Retornar uma cópia da DDLL correspondente à chave
int cpLista(pWebList pWL, int chave, ppDDLL retorno) {
    if(pWL == NULL || chave < 0 || chave >= pWL->num_nos_folha) {
        return FAIL;
    }

    NoArvoreWebList *no_folha = pWL->nos_folha[chave];
    if(no_folha == NULL || no_folha->eh_folha == 0 || no_folha->no_weblist == NULL || no_folha->no_weblist->lista == NULL) {
        return FAIL;
    }

    // Criar uma cópia da DDLL
    int res = cDDLL(retorno, pWL->sizedata);
    if(res != SUCCESS) {
        return FAIL;
    }

    // Implementar uma função para copiar a DDLL
    // copyDDLL(pDDLL origem, pDDLL destino)
    
    // res = copyDDLL(no_folha->no_weblist->lista, *retorno, pWL->sizedata);
    if(res != SUCCESS) {
        dDDLL(retorno);
        return FAIL;
    }

    return SUCCESS;
}