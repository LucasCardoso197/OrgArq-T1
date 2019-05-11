#ifndef _ORGARQ_LISTAREMOCAO_H_
#define _ORGARQ_LISTAREMOCAO_H_

#include <stdio.h>

#define offset_topo_lista 1
#define offset_tam_registro 1
#define offset_encadeamento_lista 5

int buscarPosLista(FILE *arq, int tam, long *prev, long *next);
int buscarPosListaEstavel(FILE *arq, int tam, long *prev, long *next);
int inserirLista(FILE *arq, long pos, int tam);
long removerLista(FILE *arq, int tam);

#endif