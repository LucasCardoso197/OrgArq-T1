#ifndef _ORGARQ_LISTAREMOCAO_H_
#define _ORGARQ_LISTAREMOCAO_H_
/* Biblioteca Responsável por controlar a lista de registros removidos de um arquivo 
		Obs: Assume-se que o byte offset é dado por um long int                      */

#include <stdio.h>
// Configurações
// Posicao do topo da lista dentro do arquivo
#define offset_topo_lista 1
// Posicao da informacao de tamanho do registro a partir do começo do registro
#define offset_tam_registro 1
// Posicao do offset de encadeamento da lista dentro do registro
#define offset_encadeamento_lista 5


/*	Busca de uma posição específica da lista
	prev deverá conter o primeiro elemento de tamanho igual ou menor a tam
	next deverá conter o primeiro elemento de tamanho maior, ou segundo igual
	Argumentos: - arq: ponteiro de arquivo que a lista de registros removidos será modificada
				- tam: argumento para a busca
				- prev deverá conter o byte offset do ultimo elemento de tamanho menor que tam
				- next deverá conter o byte offset do primeiro elemento de tamanho maior ou igual
	Retorna 0 para lista vazia
			1 para tam <= ao primeiro elemento da lista e next é o primeiro elemento
			2 para tam > ao primeiro elemento da lista e next é o primeiro elemento
			3 para prev e next possue elementos no meio da lista ou prev é o ultimo
					elemento da lista, que é >= a tam
			4 para prev é o ultimo elemento da lista e seu tamanho é menor que tam	*/
int buscarPosLista(FILE *arq, int tam, long *prev, long *next);
/*  Similar a função acima, mesmos valores de retorno e argumentos.
	Unica diferença:
		prev deverá conter o byte offset do último elemento de tamanho igual ou menor a tam
		next deverá conter o byte offset do primeiro elemento de tamanho maior que tam */
int buscarPosListaEstavel(FILE *arq, int tam, long *prev, long *next);


/*  Insere um registro na lista de acordo com seu tamanho de forma ordenada e crescente.
	Argumentos:
		- arq: ponteiro de arquivo que a lista de registros removidos será modificada
		- pos: byteoffset do registro a ser inserido
		- tam: tamanho do registro a ser inserido
	Retorna 0 em caso de sucesso														*/
int inserirLista(FILE *arq, long pos, int tam);


/*  Remove um registro na lista de acordo com seu tamanho. Remove o primeiro elemento da
	lista que possui tamanho maior ou igual a tam.
	Argumentos:
		- arq: ponteiro de arquivo que a lista de registros removidos será modificada
		- tam: argumento para remoção													*/
long removerLista(FILE *arq, int tam);

#endif