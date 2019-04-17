#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "orgarq_servidor.h"
#include "orgarq_cabecalho.h"

/*  Funcao que responde a funcionalidade 1
	Recebe strings com os nomes do arquivo.csv de entrada e .bin de saida
	Retorna 0 para sucesso e >0 em caso de erro	*/
int gerarArquivoSaida(char *inputFileName, char *outputFileName);

/*	Funcao que responde a funcionalidade 2
	Recebe a string com o nome do arquivo.bin que sera lido
	Retorna 0 caso sucesso e >0 em caso de erro	*/
int mostrarArquivo(char *outputFileName);

/*	Funcao que responde a funcionalidade 3
	Recebe a string com nome do arquivo.bin que sera lido
		uma string que informa o campo que sera buscado
		e uma string com o valor desse campo
	Retorna 0 caso sucesso e >0 em caso de erro	*/
int mostrarRegistros(char *inputFileName, char *nomeCampo, char *argumento);

int main(){
	char inputFileName[50];
	char outputFileName[50];
	char nomeCampo[40];
	char argumentos[MAX_TAM_CAMPO];
	int funcionalidade, i;

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
			scanf("%s ", inputFileName);
			scanf("%s ", nomeCampo);
			fgets(argumentos, 200, stdin);
			for(i=0; i < strlen(argumentos); i++)
			// Removendo tabulacoes verticais ou caractere de nova linha do argumento
				if(argumentos[i] == 13 || argumentos[i] == '\n')
					argumentos[i] = '\0';
			mostrarRegistros(inputFileName, nomeCampo, argumentos);
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
	// Tamanho maximo dos campos variaveis definidos com um macro
	// no arquivo orgarq_servidor.h; Considerado 200.
	data1.nomeServidor = malloc(MAX_TAM_CAMPO);
	data1.cargoServidor = malloc(MAX_TAM_CAMPO);
	data2.nomeServidor = malloc(MAX_TAM_CAMPO);
	data2.cargoServidor = malloc(MAX_TAM_CAMPO);

	outputFile = fopen(outputFileName, "wb");
	// Teste de sucesso de abertura do arquivo de entrada .csv
	if(outputFile == NULL){
		fprintf(stderr, "Error creating output file.\n");
		return 1;
	}
	inputFile = fopen(inputFileName, "r");
	// Teste de sucesso de abertura do arquivo de saida .bin
	if(inputFile == NULL){
		//fprintf(stderr, "Error opening %s file.\n", inputFileName);
		fprintf(stdout, "Falha no carregamento do arquivo.\n");
		return 1;
	}

	// Declaracao variaveis para leitura e escrita
	int fill = 0, pos = 0;
	char status = '0';
	char line[256];

	campoCabecalho *cab;
	// Leitura da descricao dos campos do arquivo.csv
	cab = parsearCabecalho(inputFile);

	// Escrita do cabecalho
	// status = '0' pois serao feitas escritas no binario 
	fwrite(&status, sizeof(char), 1, outputFile);
	escreverCabecalho(outputFile, cab);

	fgets(line, 256, inputFile);
	parsearDadosServidor(line, &data1);
	// Leitura linha por linha dos dados do arquivo .csv
	while(fgets(line, 256, inputFile)){
		parsearDadosServidor(line, &data2);
		fill = pos + tamanhoRegServidor(&data1)+5;
		// Verifica se o tamanho dos dois ultimos registros a serem escritos
		// irao ultrapassar o tamanho da pagina de disco, dividindo o segundo
		// entre diferentes paginas de disco
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

	// Atualiza status para '1' apos finalizacao da escrita no arquivo
	status = '1';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);
	printf("%s", outputFileName);

	free(cab);
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
	
	inputFile = fopen(inputFileName, "r");
	// Teste de sucesso de abertura do arquivo de entrada .bin
	if(inputFile == NULL){
		//fprintf(stderr, "Error opening %s file.\n", inputFileName);
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		return 1;
	}

	// Verificando integridade do arquivo
	fread(&status, sizeof(char), 1, inputFile);
	if(status == '0'){
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		return 2;
	}

	// Alocacao caso nenhum erro tenha sido encontrado
	data.nomeServidor = (char *) malloc(MAX_TAM_CAMPO*sizeof(char));
	data.cargoServidor = (char *) malloc(MAX_TAM_CAMPO*sizeof(char));

	// Posiciona na primeira pagina de disco de dados
	fseek(inputFile, TAM_PAGDISCO, SEEK_SET);
	byteCount += 32000;
	while(1){
		// Utilizando valor retornado pela funcao lerRegistro para
		// guardar quantidade de bytes lidos.
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
		printf("Número de páginas de disco acessadas: %d\n", (byteCount/TAM_PAGDISCO)+1);

	free(data.nomeServidor);
	free(data.cargoServidor);
	fclose(inputFile);
	return 0;
}

int mostrarRegistros(char *inputFileName, char *nomeCampo, char *argumento){
	FILE *inputFile;
	Servidor data;
	char status = '0';
	campoCabecalho cab[5];
	int bytesLidos = 0, bytesTotais = 0, registros = 0, isUnique = 0;

	inputFile = fopen(inputFileName, "r");
	if(inputFile == NULL){
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		return 1;
	}

	fread(&status, sizeof(char), 1, inputFile);
	if(status == '0'){
		fprintf(stdout, "Falha no processamento do arquivo.\n");
		fclose(inputFile);
		return 2;
	}

	if(!strcmp(nomeCampo, "idServidor")) isUnique = 1;
	data.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	data.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);

	// Leitura de metadados com descricao dos campos
	lerCabecalho(cab, 5, inputFile);

	// Posiciona no inicio da primeira pagina de dados do arquivo
	fseek(inputFile, 32000, SEEK_SET);
	bytesTotais = 32000;

	// Leitura dos registros
	while((bytesLidos = lerRegistro(inputFile, &data)) > 0){
		bytesTotais += bytesLidos;
		// Valida se o valor do campo do registro eh igual ao valor dado no argumento
		if(testarCampo(&data, nomeCampo, argumento)){
			imprimirCamposServidor(&data, cab);
			registros++;
			// Se o campo for considerado único, a leitura é interrompida
			// ao encontrar o primeiro registro válido
			if(isUnique == 1) break;
		}
	}

	// Imprime de acordo com o numero de registros validos
	if(registros == 0) printf("Registro inexistente.\n");
	else printf("Número de páginas de disco acessadas: %d\n", (bytesTotais/TAM_PAGDISCO)+1);

	fclose(inputFile);
	free(data.nomeServidor);
	free(data.cargoServidor);

	return 0;
}