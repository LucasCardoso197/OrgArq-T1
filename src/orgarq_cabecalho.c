#include "orgarq_cabecalho.h"

void setarCampo(campoCabecalho *camp, const char c, const char *desc){
	int i;
	camp->tag = c;
	strcpy(camp->descricao, desc);
	// Preenche os bytes que teriam lixo com @
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

	// Leitura da linha com as descricoes dos campos do arquivo .csv
	fgets(line, 256, inputFile);
	for(i=0; i<4; i++){
		// Le cada descricao ate encontrar ','
		sscanf(cptr, "%[^,]", desc);
		setarCampo(&cabecalho[i], tags[i], desc);
		cptr += strlen(desc)+1;
	}
	// Ultima descricao lida ate encontrar simbolo de nova linha
	sscanf(cptr, "%[^\n]", desc);
	setarCampo(&cabecalho[i], tags[i], desc);

	return cabecalho;
}

void escreverCabecalho(FILE* targetFile, campoCabecalho *cabCampos){
	char trash = '@';
	int i;

	// Inicializacao do valor de topo da lista de removidos
	long int topoLista = -1;

	// Escrita do cabecalho
	fwrite(&topoLista, sizeof(long int), 1, targetFile);
	escreverCampos(cabCampos, 5, targetFile);
	// Preenchendo a pagina de disco do cabecalho com '@'
	for(i = 0; i<TAM_PAGDISCO-TAM_CABECALHO; i++){
		fwrite(&trash, sizeof(char), 1, targetFile);
	}
}

void escreverCampos(campoCabecalho *cabCampos, int tamanho, FILE* target){
	int i;
	for(i=0; i<tamanho; i++){
		fwrite(&(cabCampos->tag), sizeof(char), 1, target);
		fwrite(&(cabCampos->descricao), sizeof(char), 40, target);
		cabCampos++;
	}
}

void lerCabecalho(campoCabecalho *cabCampos, int tamanho, FILE *inputFile){
	int i;
	// Guarda posicao inicial do ponteiro de arquivo para futura recuperacao
	int location = ftell(inputFile);

	// Pulando bytes de status e topo lista de removidos
	fseek(inputFile, 9, SEEK_SET);
	for(i=0; i<tamanho; i++){
		fread(&(cabCampos->tag), sizeof(char), 1, inputFile);
		fread(&(cabCampos->descricao), sizeof(char), 40, inputFile);
		cabCampos++;
	}

	// Retorna o ponteiro de arquivo para o valor inicial
	fseek(inputFile, location, SEEK_SET);
}