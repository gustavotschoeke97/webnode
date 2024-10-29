#include "DDLL_pri.h"
#include "stdlib.h"
#include "string.h"

int cDDLL(ppDDLL pp, int sizedata) {
  // alocar memória para a DDLL
  *pp = (pDDLL)malloc(sizeof(DDLL));

  // verificar
  if (*pp == NULL) {
    return FAIL;
  }

  // inicializar valores
  (*pp)->sizedata = sizedata;
  (*pp)->inicio = NULL;
  (*pp)->fim = NULL;
  (*pp)->total = 0;

  return SUCCESS;
}

int dDDLL(ppDDLL pp) {
  // verificar
  if (*pp == NULL) {
    return FAIL;
  }

  // liberar todos os nós da lista
  Node *atual = (*pp)->inicio;
  while (atual != NULL) {
    Node *proximo = atual->proximo;
    free(atual->data);
    free(atual);
    atual = proximo;
  }

  // liberar a estrutura DDLL
  free(*pp);
  *pp = NULL;

  return SUCCESS;
}

int cleanDDLL(pDDLL p) {
  // verificar
  if (p == NULL) {
    return FAIL;
  }

  Node *atual = p->inicio;

  // percorrer e liberar todos os nós
  while (atual != NULL) {
    Node *proximo = atual->proximo;
    free(atual->data);
    free(atual);
    atual = proximo;
  }

  // resetar variáveis
  p->inicio = NULL;
  p->fim = NULL;
  p->total = 0;

  return SUCCESS;
}

int iBegin(pDDLL p, void *element) {
  // verificar
  if (p == NULL || element == NULL) {
    return FAIL;
  }

  // alocar memória para o nó
  Node *novo_node = (Node *)malloc(sizeof(Node));
  if (novo_node == NULL) {
    return FAIL;
  }

  // alocar memória para o dado
  novo_node->data = malloc(p->sizedata);
  if (novo_node->data == NULL) {
    free(novo_node);
    return FAIL;
  }

  // copiar o dado
  memcpy(novo_node->data, element, p->sizedata);

  // ajustar ponteiros
  novo_node->anterior = NULL;
  novo_node->proximo = p->inicio;

  // [dado_novo]<-[dado_anterior]
  if (p->inicio != NULL) {
    p->inicio->anterior = novo_node;
  }

  p->inicio = novo_node;

  if (p->fim == NULL) {
    p->fim = novo_node;
  }

  p->total++;

  return SUCCESS;
}

int iEnd(pDDLL p, void *element) {
  // verificar
  if (p == NULL || element == NULL) {
    return FAIL;
  }

  Node *novo_node = (Node *)malloc(sizeof(Node));
  if (novo_node == NULL) {
    return FAIL;
  }

  // alocar memória para o dado
  novo_node->data = malloc(p->sizedata);
  if (novo_node->data == NULL) {
    free(novo_node);
    return FAIL;
  }

  // copiar o elemento
  memcpy(novo_node->data, element, p->sizedata);

  novo_node->anterior = p->fim;
  novo_node->proximo = NULL;

  //  [dado_antigo]->[dado_novo]
  if (p->fim != NULL) {
    p->fim->proximo = novo_node;
  }

  p->fim = novo_node;

  if (p->inicio == NULL) {
    p->inicio = novo_node;
  }

  p->total++;

  return SUCCESS;
}

int iPosition(pDDLL p, int N, void *element) {
  // verificar
  if (p == NULL || element == NULL || N < 0 || N > p->total) {
    return FAIL;
  }

  // se for no início
  if (N == 0) {
    return iBegin(p, element);
  }

  // se for no fim
  if (N == p->total) {
    return iEnd(p, element);
  }

  // alocar memória para o nó
  Node *novo_node = (Node *)malloc(sizeof(Node));
  if (novo_node == NULL) {
    return FAIL;
  }

  // alocar memória para o dado
  novo_node->data = malloc(p->sizedata);
  if (novo_node->data == NULL) {
    free(novo_node);
    return FAIL;
  }

  // copiar o dado
  memcpy(novo_node->data, element, p->sizedata);

  Node *atual = p->inicio;

  // caminhar até a posição desejada
  for (int i = 0; i < N - 1; i++) {
    atual = atual->proximo;
  }
  // [1] | [2]
  // [1]<-[novo]->[2]
  // ponteiros
  novo_node->proximo = atual->proximo;
  novo_node->anterior = atual;

  // ->[novo]<-
  if (atual->proximo != NULL) {
    atual->proximo->anterior = novo_node;
  }

  atual->proximo = novo_node;

  p->total++;

  return SUCCESS;
}

int rBegin(pDDLL p, void *element) {
  // verificar
  if (p == NULL || p->inicio == NULL) {
    return FAIL;
  }

  // pegar o elemento do início
  Node *para_remover = p->inicio;
  if (element != NULL) {
    memcpy(element, para_remover->data, p->sizedata);
  }

  p->inicio = para_remover->proximo;

  if (p->inicio != NULL) {
    p->inicio->anterior = NULL;
  } else {
    p->fim = NULL;
  }

  free(para_remover->data);
  free(para_remover);

  p->total--;

  return SUCCESS;
}

int rEnd(pDDLL p, void *element) {
  // verificar
  if (p == NULL || p->fim == NULL) {
    return FAIL;
  }

  Node *para_remover = p->fim;

  if (element != NULL) {
    memcpy(element, para_remover->data, p->sizedata);
  }

  p->fim = para_remover->anterior;

  if (p->fim != NULL) {
    p->fim->proximo = NULL;
  } else {
    p->inicio = NULL;
  }

  free(para_remover->data);
  free(para_remover);

  p->total--;

  return SUCCESS;
}

int rPosition(pDDLL p, int N, void *element) {
  // verificar
  if (p == NULL || N < 0 || N >= p->total) {
    return FAIL;
  }

  if (N == 0) {
    return rBegin(p, element);
  }

  if (N == p->total - 1) {
    return rEnd(p, element);
  }

  Node *atual = p->inicio;
  for (int i = 0; i < N; i++) {
    atual = atual->proximo;
  }

  if (element != NULL) {
    memcpy(element, atual->data, p->sizedata);
  }

  atual->anterior->proximo = atual->proximo;
  if (atual->proximo != NULL) {
    atual->proximo->anterior = atual->anterior;
  }

  free(atual->data);
  free(atual);

  p->total--;

  return SUCCESS;
}

int sBegin(pDDLL p, void *element) {
  if (p == NULL || p->inicio == NULL) {
    return FAIL;
  }

  memcpy(element, p->inicio->data, p->sizedata);

  return SUCCESS;
}

int sEnd(pDDLL p, void *element) {
  if (p == NULL || p->fim == NULL) {
    return FAIL;
  }

  memcpy(element, p->fim->data, p->sizedata);

  return SUCCESS;
}

int sPosition(pDDLL p, int N, void *element) {
  if (p == NULL || N >= p->total) {
    return FAIL;
  }

  Node *atual = p->inicio;
  for (int i = 0; i < N; i++) {
    atual = atual->proximo;
  }

  memcpy(element, atual->data, p->sizedata);

  return SUCCESS;
}

int empty(pDDLL p) { return (p == NULL || p->total == 0) ? 1 : 0; }

int countElements(pDDLL p) { return (p != NULL) ? p->total : 0; }
