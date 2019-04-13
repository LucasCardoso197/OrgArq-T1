#include "orgarq_cabecalho.h"

void setarCampo(campoCabecalho *camp, const char c, const char *desc){
	int i;
	camp->tag = c;
	strcpy(camp->descricao, desc);
	for(i = strlen(camp->descricao)+1; i < 40; i++){
		camp->descricao[i] = '@';
	}
}

campoCabecalho *parsearCabecalho(FILE *inputFile){
	const char tags[] = {'i', 's', 't', 'n', 'c'};
	char desc[40];
	char line[256], *cptr = line;
	campoCabecalho *cabecalho = malloc(sizeof(campoCabecalho)*5);
	int i;

	fgets(line, 256, inputFile);
	for(i=0; i<4; i++){
		sscanf(cptr, "%[^,]", desc);
		setarCampo(&cabecalho[i], tags[i], desc);
		cptr += strlen(desc)+1;
	}
	sscanf(cptr, "%[^\n]", desc);
	setarCampo(&cabecalho[i], tags[i], desc);

	return cabecalho;
}

void escreverCabecalho(FILE* targetFile, campoCabecalho *cabCampos){
	char trash = '@';
	int i;

	// Declaracoes e atribuicoes para arquivo de saida
	long int topoLista = -1;

	// Escrita do cabecalho
	fwrite(&topoLista, sizeof(long int), 1, targetFile);
	escreverCampos(cabCampos, 5, targetFile);
	// Preenchendo a pagina de disco do cabecalho com '@'
	for(i = 0; i<TAM_PAGDISCO-TAM_CABECALHO; i++){
		fwrite(&trash, sizeof(char), 1, targetFile);
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