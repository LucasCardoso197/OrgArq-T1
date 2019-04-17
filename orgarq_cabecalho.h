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

/*	Funcao que insere valores de tag e descricao em um registro de campo
	de cabecalho. Substituindo valores de 'lixo' por @.	
	Argumentos: - cabCampos: ponteiro para o registro de campo de cabecalho
				- c: caractere da tag do campo
				- desc: array de 40 caracteres que descrevem o campo cabecalho */
void setarCampo(campoCabecalho *cabCampos, const char c, const char *desc);

/*	Funcao que le linha de um arquivo de entrada .csv
	e converte a descricao dos cabecalhos para um array de registro	
	Retorna o endereço desse array	*/
campoCabecalho *parsearCabecalho(FILE *inputFile);

/*	Funcao que escreve a pagina de cabecalho do arquivo com base em um array de campos tag-descricao
	considera inicializacao do topoLista como -1. Preenche a pagina com @.
	Argumentos: - targetFile: ponteiro de arquivo de saida
				- cabCampos: array com 5 campos de cabecalho a serem escritos	*/
void escreverCabecalho(FILE* targetFile, campoCabecalho *cabCampos);

/*	Funcao que escreve somente um certo numero de campos de cabecalho (tag-descricao)
	Argumentos: - targetFile: ponteiro de arquivo de saida
				- lista: array com os campos de cabecalho a serem escritos
				- tamanho: numero de campos de cabecalho a serem escritos	*/
void escreverCampos(campoCabecalho *lista, int tamanho, FILE* targetFile);

/*	Funcao que le os valores dos campos de cabecalho de um arquivo, considera os campos
	sendo tag(char*1) e descricao(40*char).
	Argumentos: - inputFile: ponteiro de arquivo de entrada
				- cabCampos: ponteiro para registro onde os dados serao colocados
				- tamanho: numero de campos de cabecalho esperados	*/
void lerCabecalho(campoCabecalho *cabCampos, int tamanho, FILE *inputFile);

#endif