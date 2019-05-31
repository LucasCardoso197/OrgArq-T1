#include "orgarq_indice_nome.h"

// Aux
void imprimirIndice_nome(const indiceNome ind){
	printf("%ld - %s\n", ind.byteoffset, ind.nomeServidor);
}

void gerarIndice_nome(void *ind, const Servidor s, long offset){
	indiceNome *ip = (indiceNome *)ind;
	ip->byteoffset = offset;
	strncpy(ip->nomeServidor, s.nomeServidor, 120);
	int i;
	for(i=strlen(ip->nomeServidor)+1; i<MAX_TAM_NOME; i++)
		ip->nomeServidor[i] = '@';
}

int compararIndice_nome(const void *indA, const void *indB){
	indiceNome *ipA, *ipB;
	ipA = (indiceNome *) indA; 
	ipB = (indiceNome *) indB;
    int aux = strcmp(ipB->nomeServidor, ipA->nomeServidor);
    if(aux == 0)
        return ipB->byteoffset - ipA->byteoffset;
    else 
	    return aux;
}

int testarChave_nome(const void *ind, const void *key){
	indiceNome *indP;
	indP = (indiceNome *)ind;
    char *keyP;
    keyP = (char *)key;
	return strcmp(indP->nomeServidor, keyP);
}

// File
void escreverCabecalhoIndices_nome(FILE *outputFile, int n){
	char caux = '0';
	fwrite(&caux, sizeof(char), 1, outputFile);
	fwrite(&n, sizeof(int), 1, outputFile);

	caux = '@';
	int i;
	for(i=5; i<TAM_PAGDISCO; i++)
		fwrite(&caux, sizeof(char), 1, outputFile);
}

int escreverIndice_nome(FILE *outputFile, void *ind){
	if(outputFile == NULL) return -1;
	if(ind == NULL) return -2;

	indiceNome *indP = (indiceNome *) ind;
	fwrite(&indP->nomeServidor, sizeof(char), MAX_TAM_NOME, outputFile);
	fwrite(&indP->byteoffset, sizeof(long), 1, outputFile);
	if(ferror(outputFile)) return -1;

	return MAX_TAM_NOME*sizeof(char)+sizeof(long);
}

int lerIndice_nome(FILE *inputFile, void *ind){
	indiceNome *indP = (indiceNome *)ind;
	fread(&indP->nomeServidor, sizeof(char), MAX_TAM_NOME, inputFile);
	fread(&indP->byteoffset, sizeof(long), 1, inputFile);

	if(ferror(inputFile)) return -2;
	if(feof(inputFile)) return 0;
	return MAX_TAM_NOME+sizeof(long);
}

int criarArquivoIndices_nome(const char *inputFileName, const char *outputFileName){
	indiceNome *vetorIndicesLidos;
	int tam;

    FILE *inputFile = fopen(inputFileName, "rb");
    if(inputFile == NULL){
		fprintf(stderr, "Falha na criação do arquivo.\n");
		return -1;
	}
    char status = '0';
    fread(&status, sizeof(char), 1, inputFile);
    if(status == '0'){
        fprintf(stderr, "Arquivo corrompido para criação.");
        return -2;
    }
	vetorIndicesLidos = (indiceNome *) gerarVetorIndices(inputFile, &tam, sizeof(indiceNome), gerarIndice_nome, compararIndice_nome);
    fclose(inputFile);

	// Criação do arquivo
	FILE *outputFile = fopen(outputFileName, "wb");
	if(outputFile == NULL){
		fprintf(stderr, "Falha na criação do arquivo.\n");
		return -1;
	}

	escreverCabecalhoIndices_nome(outputFile, tam);
	escreverArquivoIndices(outputFile, tam, sizeof(indiceNome), vetorIndicesLidos, escreverIndice_nome);

	// Atualização do status após fim da escrita de indices
	status = '1';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);

	fclose(outputFile);
	return 0;
}

int lerTamanho_nome(FILE *inputFile){
	long startPos = ftell(inputFile);

	int tam;
	fseek(inputFile, 1, SEEK_SET);
	fread(&tam, sizeof(int), 1, inputFile);

	fseek(inputFile, startPos, SEEK_SET);
	return tam;
}

indiceNome *carregarArquivoIndices_nome(const char *inputFileName, int *tam){
	FILE *inputFile = fopen(inputFileName, "rb");
	if(inputFile == NULL){
		fprintf(stderr, "Falha na abertura do arquivo para carregamento.\n");
		return NULL;
	}

	char status = '0';
	fread(&status, sizeof(char), 1, inputFile);
	if(status == '0'){
		fprintf(stderr, "Arquivo corrompido.\n");
		return NULL;
	}

    *tam = lerTamanho_nome(inputFile);
	indiceNome *vetor;
	fseek(inputFile, TAM_PAGDISCO, SEEK_SET);
	vetor = carregarIndices(inputFile, sizeof(indiceNome), *tam, lerIndice_nome);

	fclose(inputFile);
	return vetor;
}

int buscarIndice_nome(const char *key, indiceNome *iArray, const int n){
    return buscarIndice(key, iArray, sizeof(indiceNome), n, testarChave_nome);
}