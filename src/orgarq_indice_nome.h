#ifndef _ORGARQ_INDICE_NOME_H
#define _ORGARQ_INDICE_NOME_H

#include "orgarq_indices.h"

#define TAM_INDICE 128UL;
#define MAX_TAM_NOME 120

struct indiceNome {
	char nomeServidor[MAX_TAM_NOME];
	long byteoffset;
};
typedef struct indiceNome indiceNome;

void gerarIndice_nome(void *ind, const Servidor s, long offset);
void imprimirIndice_nome(const indiceNome ind);
int compararIndice_nomeSort(const void *indA, const void *indB);
int compararIndice_nome(const void *indA, const void *indB);
void escreverCabecalhoIndices_nome(FILE *outputFile, int n);
int criarArquivoIndices_nome(const char *inputFileName, const char *outputFileName);
indiceNome *carregarArquivoIndices_nome(const char *inputFileName, int *tam);
int buscarIndice_nome(const char *key, indiceNome *iArray, int n);
int inserirIndice_nome(indiceNome novo, indiceNome *iArray, int *n);
void reescreverArquivoIndice_nome(FILE *indexFile, int tam, indiceNome *iArray);
int removerIndice_nome(indiceNome *ind, indiceNome *iArray, int *n);

#endif