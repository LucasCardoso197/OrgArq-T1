#ifndef _ORGARQ_INDICE_H
#define _ORGARQ_INDICE_H

#include "orgarq_servidor.h"
#include "merge_sort.h"

void *gerarVetorIndices(FILE *dataFile, int *n, unsigned long memsize, void (*gerarIndice)(void *, const Servidor, long), int (*fcmp)(const void *, const void *));
int escreverArquivoIndices(FILE *outputFile, int n, unsigned long memsize, void *vIndices, int (*escreverIndice)(FILE *, void *indice));
void *carregarIndices(FILE *inputFile, unsigned long memsize, int n, int (*lerIndice)(FILE *, void *));
int buscarChave(const void *key, void *iArray, unsigned long memsize, int n, int (*testarChave)(const void *, const void *));
int inserirIndice(void *ind, void *iArray, unsigned long memsize, int *n, int (*fcmp)(const void*, const void*));
int removerIndice(void *ind, void *iArray, unsigned long memsize, int *n, int (*fcmp)(const void*, const void*));

#endif