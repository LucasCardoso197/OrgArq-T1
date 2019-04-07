#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orgarq_servidor.h"
#include "orgarq_cabecalho.h"


size_t gerarArquivoSaida(char *inputFileName, char *outputFileName);
size_t mostrarArquivo(char *outputFileName);

int main(){
	FILE *inputFile;
	char inputFileName[50];
	char outputFileName[50];
	int funcionalidade;

	scanf("%d", &funcionalidade);
	switch (funcionalidade)
	{
		case 1:
			scanf("%s", inputFileName);
			strcpy(outputFileName, "output.bin");
			gerarArquivoSaida(inputFileName, outputFileName);
			break;
		case 2:
			scanf("%s", outputFileName);
			mostrarArquivo(outputFileName);
			break;
		case 3:
			break;
	
		default:
			printf("Funcionalidade desconhecida.\n");
			break;
	}

	return 0;
}

size_t gerarArquivoSaida(char *inputFileName, char *outputFileName){
	// Declaracoes e atribuicoes para arquivo de saida
	FILE *outputFile;
	FILE *inputFile;

	Servidor data1, data2;
	data1.nomeServidor = malloc(200);
	data1.cargoServidor = malloc(200);
	data2.nomeServidor = malloc(200);
	data2.cargoServidor = malloc(200);

	outputFile = fopen(outputFileName, "wb");
	if(outputFile == NULL){
		fprintf(stderr, "Error creating output file.\n");
		return 1;
	}
	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		fprintf(stderr, "Error opening %s file.\n", inputFileName);
		return 1;
	}

	// Escrita do cabecalho
	escreverCabecalho(outputFile);

	// Declaracao variaveis para leitura e escrita
	int fill = 0, pos = 0;
	char trash = '@';
	char line[256];

	// Pular linha com o nome dos campos do csv
	fgets(line, 256, inputFile);

	fgets(line, 256, inputFile);
	parsearDadosServidor(line, &data1);
	// Leitura linha por linha dos dados do arquivo .csv
	while(fgets(line, 256, inputFile)){
		parsearDadosServidor(line, &data2);
		fill = pos + tamanhoRegServidor(&data1)+5;
		if((fill+tamanhoRegServidor(&data2)+5) > TAM_PAGDISCO){
			fill = TAM_PAGDISCO - fill;
			escreverRegistro(&data1, outputFile, fill);
			pos = 0;
		}
		else{
			pos += escreverRegistro(&data1, outputFile, 0);
			printf("AT %d\n", pos);
		}
		copiarServidor(&data1, &data2);
	}
	escreverRegistro(&data1, outputFile, 0);

	fclose(inputFile);
	fclose(outputFile);
	free(data1.nomeServidor);
	free(data1.cargoServidor);
	free(data2.nomeServidor);
	free(data2.cargoServidor);	
}

size_t mostrarArquivo(char *inputFileName){
	size_t result;
	FILE *inputFile;
	Servidor data;
	data.nomeServidor = malloc(200);
	data.cargoServidor = malloc(200);
	
	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		fprintf(stderr, "Error opening %s file.\n", inputFileName);
		return 1;
	}
	fseek(inputFile, 32000, SEEK_SET);
	while(ftell(inputFile) < 524512){
		result = lerRegistro(inputFile, &data);
		imprimirServidor(&data);
	}

	free(data.nomeServidor);
	free(data.cargoServidor);
	fclose(inputFile);
	return result;
}