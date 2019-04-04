#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orgarq_servidor.h"

int main(){
	FILE *inputFile;
	char inputFileName[30];
	char line[256];
	
	Servidor data;
	data.nomeServidor = malloc(1000);
	data.cargoServidor = malloc(1000);

	scanf("%s", inputFileName);

	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		fprintf(stderr, "Error opening %s file.\n", inputFileName);
		return 0;
	}
	else {
		fgets(line, 256, inputFile);
		while(fgets(line, 256, inputFile)){
			resetarServidor(&data);
			printf("\n%s", line);
			parsearDadosServidor(line, &data);
			imprimirServidor(&data);
		}
	}

	free(data.nomeServidor);
	free(data.cargoServidor);

	return 0;
}