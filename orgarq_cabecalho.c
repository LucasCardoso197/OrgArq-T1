#include "orgarq_cabecalho.h"

void setarCampo(campoCabecalho *camp, const char c, const char *desc){
	int i;
	camp->tag = c;
	strcpy(camp->descricao, desc);
	for(i = strlen(camp->descricao)+1; i < 40; i++){
		camp->descricao[i] = '@';
	}
}

void escreverCampos(campoCabecalho *lista, int tamanho, FILE* target){
	int i;
	for(i=0; i<tamanho; i++){
		fwrite(&(lista->tag), sizeof(char), 1, target);
		fwrite(&(lista->descricao), sizeof(char), 40, target);
		lista++;
	}
}


void escreverCabecalho(FILE* targetFile){
	const char tags[] = {'i', 's', 't', 'n', 'c'};
	const char *descricoes[] = {"numero de identificacao do servidor",
						 "salario do servidor",
						 "telefone celular do servidor",
						 "nome do servidor",
						 "cargo do servidor"};
	char trash = '@';
	campoCabecalho cabecalho[5];
	int i;

	// Declaracoes e atribuicoes para arquivo de saida
	for(i=0; i<5; i++){
		setarCampo(&cabecalho[i], tags[i], descricoes[i]);
	}
	char status = 0;
	long int topoLista = -1;
	// Escrita do cabecalho
	fwrite(&status, sizeof(char), 1, targetFile);
	fwrite(&topoLista, sizeof(long int), 1, targetFile);
	escreverCampos(cabecalho, 5, targetFile);
	// Preenchendo a pagina de disco do cabecalho com '@'
	for(i = 0; i<TAM_PAGDISCO-TAM_CABECALHO; i++){
		fwrite(&trash, sizeof(char), 1, targetFile);
	}
}