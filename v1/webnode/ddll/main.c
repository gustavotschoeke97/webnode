/* Arquivo EXEMQLO DE AQlicacao */
// MODIFIQUEM-o, CORRIJAM-o!
#include "main.h"

int main(int argc, char *argv[]) {
  pDDLL lista = NULL;
  int ret = 0;
  int num = 0;
  int *arg;
  int rlines = 0;

  if (!fscanf(stdin, "%i", &rlines)) {
    return 1;
  }

  arg = (int *)malloc(sizeof(int) * (rlines));

  if (cDDLL(&lista, sizeof(int))) {
    printf("    FAIL: Erro ao criar a lista.\n");
  } else
    printf("    OK : Lista criada com SUCCESSo.\n");

  for (int i = 0; i < rlines; i++) {
    if (!fscanf(stdin, "%i", &arg[i])) {
      return 1;
    }
    if (iEnd(lista, &arg[i]) == SUCCESS) {
      printf("    OK :inserido com SUCCESSo(%d).\n", arg[i]);
    } else {
      printf(".    FAIL: falha na insercao.\n");
    }
  }

  if (dDDLL(&lista) == SUCCESS)
    printf("    OK: Lista destroida com SUCCESSo.\n");
  else
    printf("    FAIL: Falha na destruição da lista.\n");

  return 0;
}
