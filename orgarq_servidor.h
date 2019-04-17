#ifndef _ORGARQ_SERVIDOR_H
#define _ORGARQ_SERVIDOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orgarq_cabecalho.h"

// Tamanho maximo dos campos de tamanho variavel nomeServidor e cargoServidor
#define MAX_TAM_CAMPO 200

typedef struct {
	int idServidor;
	double salarioServidor;
	char telefoneServidor[14], *nomeServidor, *cargoServidor;
} Servidor;

/* 	Funcao responsavel pela impressao dos dados do servidor em uma unica linha
	Imprime: id salario telefone tamanhoDoNome(sem \0) nome tamanhoDoCargo(sem \0) e cargo
	Argumentos: - s: servidor a ser impresso	*/
void imprimirLinhaServidor(Servidor *s);

/*	Funcao responsavel pela impressao dos dados de um servidor com descricao
	dos campos de acordo com os valores do cabecalho
	Imprime: descricao do campo: valor do campo
	para cada campo do servidor	
	Argumentos:	- s: servidor a ser impresso
				- cabecalho: vetor com descricao e tags dos cabecalhos	*/
void imprimirCamposServidor(Servidor *s, campoCabecalho *cabecalho);

/*	Inicializa todos os campos do servidor com um valor nulo apropriado
	Garante inicializacao dos lugares onde teriam lixo o valor @
	Argumentos: - s: servidor a ser inicializado	*/
void resetarServidor(Servidor *s);

/* 	Funcao que copia valores de uma instanciacao de servidor para outra
	Argumentos: - s: ponteiro do servidor que contem os campos a serem copiados
				- target: ponteiro do servidor que ira receber os dados	*/
void copiarServidor(Servidor *target, Servidor *s);

/*	Funcao que retorna o tamanho que um registro conforma a especificacao teria
	Considera: 8 bytes encadeamento lista, 4 bytes idServidor, 8 bytes salarioServidor,
		14 bytes telefoneServidor, 4 bytes indicador de tamanho do nome, 1 byte tag nome,
		N bytes de acordo com o nome contido em s, 4 bytes indicador de tamanho do cargo,
		1 byte tag cargo, M bytes de acordo com o cargo contido em s
	Argumentos: - s: servidor com dados a serem configurados	*/
int tamanhoRegServidor(Servidor *s);

/*	Funcao que pega uma linha de um arquivo .csv(comma separated values)
	e interpreta seus dados, colocando na estrutura do servidor
	Argumentos: - s: servidor que recebe os dados	*/
void parsearDadosServidor(char *line, Servidor *s);

/*	Escreve um registro de servidor em um arquivo
	Argumentos: - s: registro com dados que serao escritos
				- target: ponteiro de arquivo onde os dados serao escritos
				- extra: quantidade extra de dados 'lixo' que serao escritos
				no final do registro, usado para preencher uma pagina de disco
	Retorna o numero de bytes escritos no arquivo	*/
int escreverRegistro(Servidor *s, FILE* target, int extra);

/*	Le um registro na posicao atual do registro do argumento
	Argumentos: - inputFile: ponteiro de arquivo do arquivo binario a ser lido
				- s: ponteiro para estrutura de servidor que recebera os valores lidos
	Retonar o numero de bytes lidos, 0 em caso de falha */
int lerRegistro(FILE *inputFile, Servidor *s);

/*	Teste se o campo do registro de servidor eh igual ao argumento
	Argumentos: - s: servidor que tera o campo comparado
				- nomeCampo: nome que identifica qual campo sera comparado
				- argumento: valor esperado que o campo deve ter em formato de texto
	Retorna 1 em caso de igualdade e 0 na diferença	*/
int testarCampo(Servidor *s, char *nomeCampo, char *argumento);

#endif