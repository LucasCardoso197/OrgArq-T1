/*	Aluno: Lucas Sobral Fontes Cardoso
	Nº USP: 8957176	*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/orgarq_servidor.h"
#include "./src/orgarq_cabecalho.h"
#include "./src/utils.h"

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


/*	Funcao que responde a funcionalidade 4
	Recebe a string com nome do arquivo.bin que sera modificado
		uma string que informa o campo que sera buscado para remocao
		e uma string com o valor desse campo
	Retorna 0 caso sucesso e >0 em caso de erro	*/
int removerServidor(char *inputFileName, char *nomeCampo, char *argumento);


/*	Funcao que responde a funcionalidade 5
	Recebe a string com nome do arquivo.bin que sera modificado
		OBS: os dados do servidor que será inserido é lido dentro
			 da função a partir da entrada padrão
	Retorna 0 caso sucesso e >0 em caso de erro	*/
int inserirServidor(char *outputFileName);


/*	Funcao que responde a funcionalidade 6
	Recebe a string com nome do arquivo.bin que sera modificado
		OBS: os dados do servidor que será inserido é lido dentro
			 da função a partir da entrada padrão
	Retorna 0 caso sucesso e >0 em caso de erro	*/
int	atualizarServidor(char *updateFileName);

int main(){
	char inputFileName[50];
	char outputFileName[50];
	char nomeCampo[40];
	char argumentos[MAX_TAM_CAMPO];
	int funcionalidade, i, result, n = 0;

	// Decisao de funcionalidade
	scanf("%d", &funcionalidade);
	switch (funcionalidade)
	{
		// Caso 1 - Escrita do arquivo binario a partir do .csv
		case 1:
			scanf("%s", inputFileName);
			strcpy(outputFileName, "arquivoTrab1.bin");
			result = gerarArquivoSaida(inputFileName, outputFileName);
			break;
		// Caso 2 - Impressao de todos os registros de um arquivo binario
		case 2:
			scanf("%s", outputFileName);
			result = mostrarArquivo(outputFileName);
			break;
		// Caso 3 - Busca de registros em arquivo binario com base em um campo
		case 3:
			scanf("%s ", inputFileName);
			scanf("%s ", nomeCampo);
			fgets(argumentos, 200, stdin);
			for(i=0; i < strlen(argumentos); i++)
			// Removendo tabulacoes verticais ou caractere de nova linha do argumento
				if(argumentos[i] == '\r' || argumentos[i] == '\n')
					argumentos[i] = '\0';
			result = mostrarRegistros(inputFileName, nomeCampo, argumentos);
			break;
		// Caso 4 - N remocoes logicas de um ou mais registros com base em um campo
		case 4:
			scanf("%s %d", inputFileName, &n);
			for(i=0; i<n; i++){
				scanf("%s ", nomeCampo);
				scan_quote_string(argumentos);
				result = removerServidor(inputFileName, nomeCampo, argumentos);
			}
			strcpy(outputFileName, inputFileName);
			break;
		// Caso 5 - N insercoes de registros no arquivo
		case 5:
			scanf("%s %d", outputFileName, &n);
			for(i=0; i<n; i++){
				result = inserirServidor(outputFileName);
			}
			break;
		// Caso 6 - N atualizacoes de registros do arquivo
		case 6:
			scanf("%s %d", outputFileName, &n);
			for(i=0; i<n; i++){
				result = atualizarServidor(outputFileName);
			}
			break;
		default:
			printf("Funcionalidade desconhecida.\n");
			break;
	}

	// Saída para erro ou sucesso
	if(result != 0)
		printf("Falha no processamento do arquivo.\n");
	else if(funcionalidade != 2 && funcionalidade != 3)
		binarioNaTela2(outputFileName);

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
	printf("%s\n", outputFileName);

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
	int resultadoLeitura, registros = 0;
	char status;
	
	inputFile = fopen(inputFileName, "r");
	// Teste de sucesso de abertura do arquivo de entrada .bin
	if(inputFile == NULL){
		//fprintf(stderr, "Error opening %s file.\n", inputFileName);
		return 1;
	}

	// Verificando integridade do arquivo
	fread(&status, sizeof(char), 1, inputFile);
	if(status == '0'){
		return 2;
	}

	// Alocacao caso nenhum erro tenha sido encontrado
	data.nomeServidor = (char *) malloc(MAX_TAM_CAMPO*sizeof(char));
	data.cargoServidor = (char *) malloc(MAX_TAM_CAMPO*sizeof(char));

	// Posiciona na primeira pagina de disco de dados
	fseek(inputFile, TAM_PAGDISCO, SEEK_SET);
	while((resultadoLeitura = lerRegistro(inputFile, &data)) != 0){
		if(resultadoLeitura > 0){
			imprimirLinhaServidor(&data);
			registros++;
		}
	}

	if(registros == 0) printf("Registro inexistente.");
	else printf("Número de páginas de disco acessadas: %ld\n", (ftell(inputFile)/TAM_PAGDISCO)+1);

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
	int registros = 0, isUnique = 0;

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

	// Leitura dos registros
	while((lerRegistro(inputFile, &data)) > 0){
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
	else printf("Número de páginas de disco acessadas: %ld\n", (ftell(inputFile)/TAM_PAGDISCO)+1);

	fclose(inputFile);
	free(data.nomeServidor);
	free(data.cargoServidor);

	return 0;
}

int removerServidor(char *inputFileName, char *nomeCampo, char *argumento){
	FILE *inputFile;
	char status = '0';
	int isUnique = 0;
	long removePosition;

	inputFile = fopen(inputFileName, "r+");
	if(inputFile == NULL){
		return 1;
	}

	fread(&status, sizeof(char), 1, inputFile);
	if(status == '0'){
		fclose(inputFile);
		return 2;
	}

	if(!strcmp(nomeCampo, "idServidor")) isUnique = 1;

	// Escrita do cabecalho
	// status = '0' pois serao feitas escritas no binario 
	status = '0';
	fseek(inputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, inputFile);

	fseek(inputFile, 32000, SEEK_SET);
	while((removePosition = buscarRegistro(inputFile, nomeCampo, argumento)) != -1){
		fseek(inputFile, removePosition, SEEK_SET);
		removerRegistro(inputFile);
		if(isUnique) break;
	}

	// Escrita do cabecalho
	// status = '1' pois todas as mudanças do arquivo acabaram
	status = '1';
	fseek(inputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, inputFile);

	fclose(inputFile);
	return 0;
}

int inserirServidor(char *outputFileName){
	Servidor inputData;
	FILE *outputFile;
	char status = '0';

	outputFile = fopen(outputFileName, "r+");
	if(outputFile == NULL){
		return 1;
	}

	fread(&status, sizeof(char), 1, outputFile);
	if(status == '0'){
		fclose(outputFile);
		return 2;
	}

	// Escrita do cabecalho
	// status = '0' pois serao feitas escritas no binario 
	status = '0';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);

	inputData.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	inputData.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	// Leitura de servidor a ser inserido da entrada padrão
	scanServidor(&inputData);

	// Chama função de inserção
	inserirRegistro(outputFile, inputData);

	// Escrita do cabecalho
	// status = '1' pois todas as mudanças do arquivo acabaram
	status = '1';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);

	free(inputData.nomeServidor);
	free(inputData.cargoServidor);
	fclose(outputFile);
	return 0;
}

int atualizarServidor(char *updateFileName){
	// Leitura de argumentos de entrada
	char campoBusca[40], campoAtualiza[40];
	char argBusca[MAX_TAM_CAMPO], argAtualiza[MAX_TAM_CAMPO];
	scanf("%s", campoBusca);
	scan_quote_string(argBusca);
	scanf("%s", campoAtualiza);
	scan_quote_string(argAtualiza);

	FILE *updateFile;
	char status = '0';
	int isUnique = 0;
	long updatePosition = 32000, readResult;
	Servidor data;

	updateFile = fopen(updateFileName, "r+");
	if(updateFile == NULL){
		return 1;
	}

	fread(&status, sizeof(char), 1, updateFile);
	if(status == '0'){
		fclose(updateFile);
		return 2;
	}

	// Escrita do cabecalho
	// status = '0' pois serao feitas escritas no binario 
	status = '0';
	fseek(updateFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, updateFile);

	if(!strcmp(campoBusca, "idServidor")) isUnique = 1;
	data.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	data.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);

	fseek(updateFile, 32000, SEEK_SET);
	while((readResult = lerRegistro(updateFile, &data)) != 0){
		// Valida se o valor do campo do registro eh igual ao valor dado no argumento
		if(testarCampo(&data, campoBusca, argBusca) && readResult > 0){
			fseek(updateFile, updatePosition, SEEK_SET);
			atualizarRegistro(updateFile, campoAtualiza, argAtualiza, &data);
			// Se o campo for considerado único, a leitura é interrompida
			// ao encontrar o primeiro registro válido
			if(isUnique == 1) break;
		}
		updatePosition = ftell(updateFile);
	}

	// Escrita do cabecalho
	// status = '1' pois todas as mudanças do arquivo acabaram
	status = '1';
	fseek(updateFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, updateFile);

	free(data.nomeServidor);
	free(data.cargoServidor);
	fclose(updateFile);
	return 0;
}