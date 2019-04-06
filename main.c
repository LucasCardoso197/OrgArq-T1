#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orgarq_servidor.h"
#include "orgarq_cabecalho.h"

int main(){
	FILE *inputFile;
	char inputFileName[30];
	char line[256];
	char trash = '@';
	int i;

	// Declaracoes e atribuicoes para arquivo de saida
	FILE *outputFile;

	Servidor data;
	data.nomeServidor = malloc(1000);
	data.cargoServidor = malloc(1000);

	scanf("%s", inputFileName);
	outputFile = fopen("output.bin", "wb");
	if(outputFile == NULL){
		fprintf(stderr, "Error creating output file.\n");
	}

	int currentOutputPos = 0;
	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		fprintf(stderr, "Error opening %s file.\n", inputFileName);
		return 0;
	}
	
	// Escrita do cabecalho
	escreverCabecalho(outputFile);

	fgets(line, 256, inputFile);
	// Leitura linha por linha dos dados do arquivo .csv
	while(fgets(line, 256, inputFile)){
		resetarServidor(&data);
		parsearDadosServidor(line, &data);
		if((currentOutputPos + tamanhoRegServidor(&data)+5) > TAM_PAGDISCO){
			while(currentOutputPos < TAM_PAGDISCO){
				fwrite(&trash, sizeof(char), 1, outputFile);
				currentOutputPos++;
			}
			currentOutputPos = 0;
		}
		currentOutputPos += escreverRegistro(&data, outputFile);
		//imprimirServidor(&data);
		//printf("%d\n", currentOutputPos);
	}
	while(currentOutputPos < TAM_PAGDISCO){
		fwrite(&trash, sizeof(char), 1, outputFile);
		currentOutputPos++;
	}

	fclose(inputFile);
	fclose(outputFile);

	free(data.nomeServidor);
	free(data.cargoServidor);

	return 0;
}