
# define SUCCESS 0
# define FAIL 1

typedef struct weblist * pweblist , ** ppweblist ;

// Funcoes operacionais
int cWL(ppweblist ppWL, int nivel, int sizedata);
int dWL(ppweblist ppWL);
int iDado(pweblist pWL, void* dado);
int pLista(pweblist pWL, void (*imprime)(void *a));

// Funcoes focada nos dados
// int iDado (...) ; // inserir um novo dado na estrutura ;
// int rDado (...) ; // remover um dado da estrutura ;
// int bDado (...) ; // buscar um dado na estrutura ;
// int pLista (...) ; // percorrer a lista de dados ;

// // Funcoes focada nos nos - folha
// int cpLista (... , int chave , ppDDLL retorno ) ; // retornar uma copia da DDLL correspondente a chave ;
// int sbLista (... , int chave , pDDLL novaLista ); // substituir a lista DDLL correspondente a chave pela lista recebida por parametro ( novaLista );
// int rmLista (... , int chave , ppDDLL rmLista ) ; // retornar a lista ’ rmLista ’ por parametro e remover a lista DDLL correspondente a chave ;
// int nvLista (... , int chave ) ; // criar uma DDLL vazia para a chave recebida como parametro ;

// // Funcoes da WebList
// int nroEleNoFolha (... , int * retorno ) ; // retornar o numero de elementos em um no - folha especifico ( soma de elementos de cada lista do no - folha )
// int nroNoFolha (... , int* retorno ); // retornar o numero total de nos - folha da estrutura
// int nroEleWL (... , int * retorno ); // retornar o numero total de elementos cadastrados na webList
// int lstChaves (... , ppDDLL retorno ); // retornar uma lista com todas as chaves da WebList .
// int WLbalanceada (...) ; // retornar SUCCESS se a webList estiver balanceada e, FAIL , caso contrario .