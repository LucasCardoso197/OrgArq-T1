#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orgarq_servidor.h"
#include "orgarq_cabecalho.h"


int gerarArquivoSaida(char *inputFileName, char *outputFileName);
int mostrarArquivo(char *outputFileName);
int mostrarRegistros(char *inputFileName, char *nomeCampo, void *value);

int main(){
	char inputFileName[50];
	char outputFileName[50];
	char nomeCampo[40];
	void *value;
	int funcionalidade;

	// Decisao de funcionalidade
	scanf("%d", &funcionalidade);
	switch (funcionalidade)
	{
		// Caso 1 - Escrita do arquivo binario a partir do .csv
		case 1:
			scanf("%s", inputFileName);
			strcpy(outputFileName, "arquivoTrab1.bin");
			gerarArquivoSaida(inputFileName, outputFileName);
			break;
		// Caso 2 - Impressao de todos os registros de um arquivo binario
		case 2:
			scanf("%s", outputFileName);
			mostrarArquivo(outputFileName);
			break;
		// Caso 3 - Busca de registros em arquivo binario com base em um campo
		case 3:
			scanf("%s", inputFileName);
			scanf("%s ", nomeCampo);
			if(strcmp(nomeCampo, "idServidor") == 0){
				value = (int *)malloc(sizeof(int));
				scanf("%d", (int *)value);
				free((int *)value);
			}
			else if(strcmp(nomeCampo, "salarioServidor") == 0){
				value = (double *)malloc(sizeof(double));
				scanf("%lf", (double *)value);
				free((double *)value);
			}
			else if(strcmp(nomeCampo, "telefoneServidor") == 0){
				value = (char *)malloc(sizeof(char)*14);
				scanf("%s", (char *)value);
				free((char *)value);
			}
			else if(strcmp(nomeCampo, "nomeServidor") == 0){
				value = (char *)malloc(sizeof(char)*200);
				scanf("%[^\n]", (char *)value);
				free((char *)value);
			}
			else if(strcmp(nomeCampo, "cargoServidor") == 0){
				value = (char *)malloc(sizeof(char)*200);
				scanf("%[^\n]", (char *)value);
				free(value);
			}
			else
				fprintf(stderr, "Erro de entrada funcionalidade 3.");
			mostrarRegistros(inputFileName, nomeCampo, value);
			break;
	
		default:
			printf("Funcionalidade desconhecida.\n");
			break;
	}

	return 0;
}

int gerarArquivoSaida(char *inputFileName, char *outputFileName){
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
		//fprintf(stderr, "Error opening %s file.\n", inputFileName);
		fprintf(stdout, "Falha no carregamento do arquivo.");
		return 1;
	}

	// Declaracao variaveis para leitura e escrita
	int fill = 0, pos = 0;
	char status = 0;
	char line[256];

	campoCabecalho *cab;
	// Leitura do nome dos campos do arquivo.csv
	cab = parsearCabecalho(inputFile);

	// Escrita do cabecalho
	fwrite(&status, sizeof(char), 1, outputFile);
	escreverCabecalho(outputFile, cab);

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
		}
		copiarServidor(&data1, &data2);
	}
	escreverRegistro(&data1, outputFile, 0);

	status = 1;
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);
	printf("%s\n", outputFileName);
	free(data1.nomeServidor);
	free(data1.cargoServidor);
	free(data2.nomeServidor);
	free(data2.cargoServidor);
	return 0;	
}

int mostrarArquivo(char *inputFileName){
	FILE *inputFile;
	Servidor data;
	int regCount = 0, byteCount = 0, resultadoLeitura;
	char status;
	
	// Abertura e verificacao do arquivo
	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		//fprintf(stderr, "Error opening %s file.\n", inputFileName);
		fprintf(stdout, "Falha no processamento do arquivo.\n");

		return 1;
	}

	// Verificando integridade do arquivo
	fread(&status, sizeof(char), 1, inputFile);
	if(status == 0){
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		return -1;
	}

	// Alocacao caso nenhum erro tenha sido encontrado
	data.nomeServidor = (char *) malloc(200*sizeof(char));
	data.cargoServidor = (char *) malloc(200*sizeof(char));

	// Posiciona na primeira pagina de disco de dados
	fseek(inputFile, TAM_PAGDISCO, SEEK_SET);
	while(1){
		resultadoLeitura = lerRegistro(inputFile, &data);
		if(resultadoLeitura < 0) {
			free(data.nomeServidor);
			free(data.cargoServidor);
			fclose(inputFile);
			return resultadoLeitura;
		}
		byteCount+= resultadoLeitura;
		if(feof(inputFile) || ferror(inputFile)) break;
		else imprimirLinhaServidor(&data);
		regCount++;
	}
	if(regCount == 0)
		printf("Registro inexistente.");
	else
		printf("Numero de paginas de disco acessadas: %d\n", (byteCount/TAM_PAGDISCO)+1);

	free(data.nomeServidor);
	free(data.cargoServidor);
	fclose(inputFile);
	return 0;
}

int mostrarRegistros(char *inputFileName, char *nomeCampo, void *value){
	FILE *inputFile;
	Servidor data;
	int regCount = 0, byteCount = 0, status = 0;

	// Abertura e verificacao do arquivo
	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		//fprintf(stderr, "Error opening %s file.\n", inputFileName);
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		return 1;
	}

	// Verificando integridade do arquivo
	fread(&status, sizeof(char), 1, inputFile);
	if(status == 0){
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		fclose(inputFile);
		return -1;
	}

	//
	data.nomeServidor = (char *) malloc(200);
	data.cargoServidor = (char *) malloc(200);

	// Posiciona na primeira pagina de disco de dados
	fseek(inputFile, TAM_PAGDISCO, SEEK_SET);
	while(1){
		byteCount+= lerRegistro(inputFile, &data);
		if(feof(inputFile) || ferror(inputFile)) break;
		else {
			if(strcmp(nomeCampo, "idServidor") == 0){
				int *id = (int *)value;
				if(data.idServidor == *id){
					imprimirCamposServidor(&data);
					regCount++;
				}
			}
			else if(strcmp(nomeCampo, "salarioServidor") == 0){
				double *salario = (double *)value;
				if(data.salarioServidor == *salario){
					imprimirCamposServidor(&data);
					regCount++;
				}
			}
			else if(strcmp(nomeCampo, "telefoneServidor") == 0){
				char *telefone = (char *)value;
				if(strncmp(data.telefoneServidor, telefone, 14) == 0){
					imprimirCamposServidor(&data);
					regCount++;
				}
			}
			else if(strcmp(nomeCampo, "nomeServidor") == 0){
				char *nome = (char *)value;
				if(strcmp(data.nomeServidor, nome) == 0){
					imprimirCamposServidor(&data);
					regCount++;
				}
			}
			else if(strcmp(nomeCampo, "cargoServidor") == 0){
				char *cargo = (char *)value;
				if(strcmp(data.cargoServidor, cargo) == 0){
					imprimirCamposServidor(&data);
					regCount++;
				}
			}
		}
	}
	if(regCount == 0)
		printf("Registro inexistente.\n");
	else
		printf("Numero de paginas de disco acessadas: %d\n", (byteCount/TAM_PAGDISCO)+1);

	free(data.nomeServidor);
	free(data.cargoServidor);
	fclose(inputFile);
	return 0;
}