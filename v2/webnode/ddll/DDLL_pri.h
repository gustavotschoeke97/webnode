#include "DDLL_pub.h"

typedef struct Node {
  void *data;
  struct Node *proximo;
  struct Node *anterior;
} Node;

typedef struct DDLL {
  struct Node *inicio;
  struct Node *fim;
  int sizedata;
  int total;
} DDLL;