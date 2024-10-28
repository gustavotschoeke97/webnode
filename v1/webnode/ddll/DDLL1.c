
#include "DDLL_pri.h"
#include "stdlib.h"
#include "string.h"

int cDDLL(ppDDLL pp, int sizedata) {
  *pp = (pDDLL)malloc(sizeof(pDDLL));

  if (*pp == NULL) {
    return FAIL;
  }

  (*pp)->sizedata = sizedata;
  (*pp)->inicio = NULL;
  (*pp)->fim = NULL;
  (*pp)->total = 0;

  return SUCCESS;
}

int dDDLL(ppDDLL pp) {
  if (pp == NULL) {
    return FAIL;
  }

  Node *atual = (*pp)->inicio;

  while (atual != NULL) {
    Node *proximo = atual->proximo;
    free(atual->data);
    free(atual);
    atual = proximo;
  }

  free(*pp);
  *pp = NULL;

  return SUCCESS;
}

int cleanDDLL(pDDLL p) {
  if (p == NULL) {
    return FAIL;
  }

  Node *atual = p->inicio;

  while (atual != NULL) {
    Node *proximo = atual->proximo;
    free(atual->proximo);
    free(atual);
    atual = proximo;
  }

  p->inicio = NULL;
  p->fim = NULL;
  p->total = 0;

  return SUCCESS;
}

int iBegin(pDDLL p, void *element) {
  if (p == NULL || element == NULL) {
    return FAIL;
  }

  Node *novo_node = (Node *)malloc(p->sizedata);

  if (novo_node == NULL) {
    return FAIL;
  }

  // copiar memoria
  memcpy(novo_node->data, element, p->sizedata);

  // ajustar ponteiros
  novo_node->anterior = NULL;
  novo_node->proximo = p->inicio;

  if (p->inicio->anterior != NULL) {
    p->inicio->anterior = novo_node;
  }

  // setar novo incio
  p->inicio = novo_node;

  // se fim for null é porque somente tem 1 item
  // setar o novo_node para o fim
  if (p->fim == NULL) {
    p->fim = novo_node;
  }
}