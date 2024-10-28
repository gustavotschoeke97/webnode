

#include "main.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main() {
  pDDLL lista;
  int ret, element, element_out;

  // Criar a lista
  ret = cDDLL(&lista, sizeof(int));
  if (ret == FAIL) {
    printf("Falha ao criar a lista\n");
    return -1;
  }

  // Inserir elementos no início
  element = 10;
  ret = iBegin(lista, &element);
  if (ret == SUCCESS) {
    printf("Inserido no início: %d\n", element);
  }

  element = 21;
  ret = iBegin(lista, &element);
  if (ret == SUCCESS) {
    printf("Inserido no início: %d\n", element);
  }

  // Inserir elementos no final
  element = 30;
  ret = iEnd(lista, &element);
  if (ret == SUCCESS) {
    printf("Inserido no final: %d\n", element);
  }

  element = 40;
  ret = iEnd(lista, &element);
  if (ret == SUCCESS) {
    printf("Inserido no final: %d\n", element);
  }

  // Inserir elemento na posição 2
  element = 25;
  ret = iPosition(lista, 2, &element);
  if (ret == SUCCESS) {
    printf("Inserido na posição 2: %d\n", element);
  }

  // Contar elementos
  printf("Total de elementos: %d\n", countElements(lista));

  // Buscar o primeiro elemento
  ret = sBegin(lista, &element_out);
  if (ret == SUCCESS) {
    printf("Primeiro elemento: %d\n", element_out);
  }

  // Remover elemento do início
  ret = rBegin(lista, &element_out);
  if (ret == SUCCESS) {
    printf("Removido do início: %d\n", element_out);
  }

  // Remover elemento do final
  ret = rEnd(lista, &element_out);
  if (ret == SUCCESS) {
    printf("Removido do final: %d\n", element_out);
  }

  // Remover elemento da posição 1
  ret = rPosition(lista, 1, &element_out);
  if (ret == SUCCESS) {
    printf("Removido da posição 1: %d\n", element_out);
  }

  // Contar elementos após remoções
  printf("Total de elementos após remoções: %d\n", countElements(lista));

  // Verificar se a lista está vazia
  if (empty(lista)) {
    printf("A lista esta vazia.\n");
  } else {
    printf("A lista não esta vazia.\n");
  }

  // Limpar a lista
  cleanDDLL(lista);

  // Destruir a lista
  dDDLL(&lista);

  printf("Fim do teste\n");

  return 0;
}
