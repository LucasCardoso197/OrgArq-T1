#ifndef _ORGARQ_CABECALHO_H_
#define _ORGARQ_CABECALHO_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAM_CABECALHO 214
#define TAM_PAGDISCO 32000

typedef struct {
	char tag;
	char descricao[40];
} campoCabecalho;

void setarCampo(campoCabecalho *camp, const char c, const char *desc);
void escreverCabecalho(FILE* targetFile);
void escreverCampos(campoCabecalho *lista, int tamanho, FILE* target);

#endif