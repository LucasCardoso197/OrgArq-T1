/*	Aluno: Lucas Sobral Fontes Cardoso
	Nº USP: 8957176	*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./src/orgarq_servidor.h"
#include "./src/orgarq_cabecalho.h"
#include "./src/orgarq_indice_nome.h"
#include "./src/utils.h"
#include "./src/merge_sort.h"

/*  Funcao que responde a funcionalidade 1
*	Recebe strings com os nomes do arquivo.csv de entrada e .bin de saida
*	Retorna 0 para sucesso e >0 em caso de erro	*/
int gerarArquivoSaida(char *inputFileName, char *outputFileName);


/*	Funcao que responde a funcionalidade 2
*	Recebe a string com o nome do arquivo.bin que sera lido
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int mostrarArquivo(char *outputFileName);


/*	Funcao que responde a funcionalidade 3
*	Recebe a string com nome do arquivo.bin que sera lido
*		uma string que informa o campo que sera buscado
*		e uma string com o valor desse campo
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int mostrarRegistros(char *inputFileName, char *nomeCampo, char *argumento);


/*	Funcao que responde a funcionalidade 4
*	Recebe a string com nome do arquivo.bin que sera modificado
*		uma string que informa o campo que sera buscado para remocao
*		e uma string com o valor desse campo
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int removerServidor(char *inputFileName, char *nomeCampo, char *argumento);


/*	Funcao que responde a funcionalidade 5
*	Recebe a string com nome do arquivo.bin que sera modificado
*		OBS: os dados do servidor que será inserido é lido dentro
*			 da função a partir da entrada padrão
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int inserirServidor(char *outputFileName);


/*	Funcao que responde a funcionalidade 6
*	Recebe a string com nome do arquivo.bin que sera modificado
*		OBS: os dados do servidor que será inserido é lido dentro
*			 da função a partir da entrada padrão
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int	atualizarServidor(char *updateFileName);

/*	Funcao que responde a funcionalidade 7
*	Recebe a string com nome do arquivo.bin que sera modificado
*	e também o nome do arquivo que deverá ser gerado ordenado
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int ordenarArquivoDeDados(const char* inputFileName, const char* outputFileName);

/*	Funcao que responde a funcionalidade 8
*	Recebe a string com nome dos arquivos.bin que serão combinados
*	e também o nome do arquivo que será gerado com dados presentes em algum deles
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int mergeArquivoDeDados(const char *inputFileName1, const char *inputFileName2, const char *outputFileName);


/*	Funcao que responde a funcionalidade 9
*	Recebe a string com nome dos arquivos.bin que serão comparados
*	e também o nome do arquivo que será gerado com dados presentes em ambos
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int matchArquivoDeDados(const char *inputFileName1, const char *inputFileName2, const char *outputFileName);

/*	Funcao que responde a funcionalidade 11
*	Recebe a string com nome dos arquivos.bin de dados e de índices
*	e também a string com o nome a ser buscado
*	Retorna 0 caso sucesso e >0 em caso de erro	*/
int buscarRegistroInd(const char *dataFileName, const char *indexFileName, const char *key);

/*	Funcao que responde a funcionalidade 12
*	Recebe a string com nome dos arquivos.bin de dados e de índices
*	e também o número de remoções que serão feitas, cujo parâmetros
*	serão recebidos da entrada padrão
*		Retorna 0 caso sucesso e >0 em caso de erro	*/
int removeRegistroInd(const char *dataFileName, const char *indexFileName, int n);

/*	Funcao que responde a funcionalidade 13
*	Recebe a string com nome dos arquivos.bin de dados e de índices
*	e também o número de inserções que serão feitas, cujo parâmetros
*	serão recebidos da entrada padrão
*		Retorna 0 caso sucesso e >0 em caso de erro	*/
int insereRegistroInd(const char *dataFileName, const char *indexFileName, int n);

int comparaBusca(const char *inputFileName, const char *inputIndiceName, char *key);

int main(){
	char inputFileName[50];
	char inputFileName2[50];
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
		case 7: 
			scanf("%s", inputFileName);
			scanf("%s", outputFileName);
			result = ordenarArquivoDeDados(inputFileName, outputFileName);
			break;
		case 8:
			scanf("%s", inputFileName);
			scanf("%s", inputFileName2);
			scanf("%s", outputFileName);
			result = mergeArquivoDeDados(inputFileName, inputFileName2, outputFileName);
			break;
		case 9:
			scanf("%s", inputFileName);
			scanf("%s", inputFileName2);
			scanf("%s", outputFileName);
			result = matchArquivoDeDados(inputFileName, inputFileName2, outputFileName);
			break;
		case 10:
			scanf("%s %s", inputFileName, outputFileName);
			result = criarArquivoIndices_nome(inputFileName, outputFileName);
			break;
		case 11:
			scanf("%s %s", inputFileName, outputFileName);
			scanf("%s ", nomeCampo);
			fgets(argumentos, 120, stdin);
			for(i=0; i < strlen(argumentos); i++)
			// Removendo tabulacoes verticais ou caractere de nova linha do argumento
				if(argumentos[i] == '\r' || argumentos[i] == '\n')
					argumentos[i] = '\0';
			result = buscarRegistroInd(inputFileName, outputFileName, argumentos);
			break;
		case 12:
			scanf("%s %s %d", inputFileName, inputFileName2, &n);
			result = removeRegistroInd(inputFileName,inputFileName2, n);
			strcpy(outputFileName, inputFileName2);
			break;
		case 13:
			scanf("%s %s %d", inputFileName, inputFileName2, &n);
			result = insereRegistroInd(inputFileName, inputFileName2, n);
			strcpy(outputFileName, inputFileName2);
			break;
		case 14:
			scanf("%s %s", inputFileName, outputFileName);
			scanf("%s ", nomeCampo);
			fgets(argumentos, 120, stdin);
			for(i=0; i < strlen(argumentos); i++)
			// Removendo tabulacoes verticais ou caractere de nova linha do argumento
				if(argumentos[i] == '\r' || argumentos[i] == '\n')
					argumentos[i] = '\0';
			result = comparaBusca(inputFileName, outputFileName, argumentos);
			break;
		default:
			printf("Funcionalidade desconhecida.\n");
			break;
	}

	// Saída para erro ou sucesso
	if(result == 3)
		printf("Registro inexistente.\n");
	else if(result != 0)
		printf("Falha no processamento do arquivo.\n");
	else if(funcionalidade != 2 && funcionalidade != 3 && funcionalidade != 11 && funcionalidade != 14)
		binarioNaTela2(outputFileName);

	return 0;
}

// Caso 1
int gerarArquivoSaida(char *inputFileName, char *outputFileName){
	// Declaracoes e atribuicoes para arquivo de saida
	FILE *outputFile;
	FILE *inputFile;

	Servidor data1, data2;
	// Tamanho maximo dos campos variaveis definidos com um macro
	// no arquivo orgarq_servidor.h; Considerado 120.
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

// Caso 2
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

// Caso 3
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
	int numPag = (ftell(inputFile)/TAM_PAGDISCO)+1;
	if(registros == 0) printf("Registro inexistente.\n");
	else printf("Número de páginas de disco acessadas: %d\n", numPag);

	fclose(inputFile);
	free(data.nomeServidor);
	free(data.cargoServidor);

	return 0;
}

// Caso 4
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

// Caso 5
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

// Caso 6
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

// Caso 7
int ordenarArquivoDeDados(const char* inputFileName,const char* outputFileName){
	// Inicialização
	FILE *inputFile, *outputFile;
	inputFile = fopen(inputFileName, "rb");
	outputFile = fopen(outputFileName, "wb");
	if(outputFile == NULL || inputFile == NULL){
		if(outputFile != NULL) fclose(outputFile);
		if(inputFile != NULL) fclose(inputFile);
		return 1;
	}

	// Validacao do arquivo de entrada
	char status = '0';
	fread(&status, sizeof(char), 1, inputFile);
	if(status == '0'){
		fclose(inputFile);
		fclose(outputFile);
		return 2;
	}

	// Leitura dos metadados do cabeçalho do arquivo de entrada
	int i;
	campoCabecalho cab[5];
	lerCabecalho(cab, 5, inputFile);
	// Carregamento dos dados do arquivo de entrada
	Servidor *inputFileData;
	int tam;
	inputFileData = carregarArquivoDados(inputFile, &tam);
	fclose(inputFile);

	// Ordenação dos dados carregados do arquivo de entrada
	MS_sort(inputFileData, tam, sizeof(Servidor), compararServidor);

	// Criação do arquivo ordenado
	// Inicializa status como 0 pois serão escritos novos registros
	status = '0';
	fwrite(&status, sizeof(char), 1, outputFile);

	//Escrita do cabeçalho, com mesmo metadados do arquivo de entrada
	escreverCabecalho(outputFile, cab);

	// Escrita dos dados
	int fill, posicaoPagDisco = 0;
	fseek(outputFile, TAM_PAGDISCO, SEEK_SET);
	for(i=0; i<tam-1; i++){
		fill = posicaoPagDisco+tamanhoRegServidor(&inputFileData[i])+tamanhoRegServidor(&inputFileData[i+1]) + 10;
		if(fill > TAM_PAGDISCO){
			fill = TAM_PAGDISCO -(fill-tamanhoRegServidor(&inputFileData[i+1])-5);
			posicaoPagDisco = -1*(tamanhoRegServidor(&inputFileData[i])+5);
		}
		else
			fill = 0;
		escreverRegistro(&inputFileData[i], outputFile, fill);
		posicaoPagDisco += tamanhoRegServidor(&inputFileData[i])+5;
	}
	escreverRegistro(&inputFileData[tam-1], outputFile, 0);

	// Atualização de status no final da execução
	status = '1';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);

	// Finalização
	liberarVetorServidor(inputFileData, tam);
	fclose(outputFile);
	return 0;
}

// Caso 8
int mergeArquivoDeDados(const char *inputFileName1, const char *inputFileName2, const char *outputFileName){
	FILE *inputFile1, *inputFile2, *outputFile;
	// Abertura dos arquivos
	char status = '0';
	inputFile1 = fopen(inputFileName1, "rb");
	inputFile2 = fopen(inputFileName2, "rb");
	outputFile = fopen(outputFileName, "wb+");
	if(inputFile1 == NULL || inputFile2 == NULL || outputFile == NULL){
		if(inputFile1 != NULL) fclose(inputFile1);
		if(inputFile2 != NULL) fclose(inputFile2);
		if(outputFile != NULL) fclose(outputFile);
		return 1;
	}

	// Validação dos arquivos de entrada
	fread(&status, sizeof(char), 1, inputFile1);
	if(status == '0'){
		fclose(inputFile1);
		fclose(inputFile2);
		fclose(outputFile);
		return 2;
	}
	fread(&status, sizeof(char), 1, inputFile2);
	if(status == '0'){
		fclose(inputFile1);
		fclose(inputFile2);
		fclose(outputFile);
		return 2;
	}

	// Leitura dos metadados do cabeçalho do arquivo de entrada 1!
	campoCabecalho cab[5];
	lerCabecalho(cab, 5, inputFile1);
	// Preparando arquivo de saída
	status = '0';
	fwrite(&status, sizeof(char), 1, outputFile);
	// Escrevemos o cabeçalho assumindo que tenho os mesmo metadados do arquivo 1!
	escreverCabecalho(outputFile, cab);

	// Merge
	Servidor registro1, registro2;
	registro1.nomeServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	registro2.nomeServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	registro1.cargoServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	registro2.cargoServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	char temRegistro1 = 1, temRegistro2 = 1;
	int aux;

	fseek(inputFile1, TAM_PAGDISCO, SEEK_SET);
	fseek(inputFile2, TAM_PAGDISCO, SEEK_SET);
	fseek(outputFile, TAM_PAGDISCO, SEEK_SET);
	// Leitura do primeiro registro valido de cada arquivo
	// Utiliza auxiliar para resolver caso onde um arquivo não tem registros
	while((aux = lerRegistro(inputFile1, &registro1)) == -1);
	if(aux == 0) temRegistro1 = 0;
	while((aux = lerRegistro(inputFile2, &registro2)) == -1);
	if(aux == 0) temRegistro2 = 0;

	// Laço percorrendo os arquivos
	while(temRegistro1 && temRegistro2){
		if(compararServidor(&registro1, &registro2) >= 0){
			// Escreve registro 1 na saída
			escreverRegistroAutoFill(&registro1, outputFile);

			// Se igual também ler registro arquivo 2
			if(compararServidor(&registro1, &registro2) == 0){
				while((aux = lerRegistro(inputFile2, &registro2)) == -1);
				if(aux == 0) temRegistro2 = 0; // Verificação final de arquivo 2
			}

			// Ler registro arquivo 1
			while((aux = lerRegistro(inputFile1, &registro1)) == -1);
			if(aux == 0) temRegistro1 = 0; // Verificação final de arquivo 1
		}
		else {
			// Escreve registro 2 na saída
			escreverRegistroAutoFill(&registro2, outputFile);

			// Ler registro 2
			while((aux = lerRegistro(inputFile2, &registro2)) == -1);
			if(aux == 0) temRegistro2 = 0; // Verificação final de arquivo 2
		}
	}
	// Acabou um arquivo, vamos percorrer o outro até acabar também
	// Posicionamos o arquvo que tem registros no inputFile 1
	// Também aproveitamos os testes para escrever o arquivo que já foi lido
	if(temRegistro2){ // swap(inputFile1, inputFile2)
		FILE *aux = inputFile1;
		inputFile1 = inputFile2;
		inputFile2 = aux;
		temRegistro1 = 1;
		escreverRegistroAutoFill(&registro2, outputFile);
		printf("File 2 has more xD.\n");
	}
	else if(temRegistro1)
		escreverRegistroAutoFill(&registro1, outputFile);

	// Percorrendo o arquivo que ainda tem dados
	while(temRegistro1){
		while((aux = lerRegistro(inputFile1, &registro1)) == -1);
		if(aux == 0) temRegistro1 = 0;
		else escreverRegistroAutoFill(&registro1, outputFile);
	}

	// Atualizando status do arquvio de saída
	status = '1';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);

	// Finalização
	free(registro1.nomeServidor);
	free(registro2.nomeServidor);
	free(registro1.cargoServidor);
	free(registro2.cargoServidor);
	fclose(inputFile1);
	fclose(inputFile2);
	fclose(outputFile);
	return 0;
}

// Caso 9
int matchArquivoDeDados(const char *inputFileName1, const char *inputFileName2, const char *outputFileName){
	FILE *inputFile1, *inputFile2, *outputFile;
	// Abertura dos arquivos
	char status = '0';
	inputFile1 = fopen(inputFileName1, "rb");
	inputFile2 = fopen(inputFileName2, "rb");
	outputFile = fopen(outputFileName, "wb+");
	if(inputFile1 == NULL || inputFile2 == NULL || outputFile == NULL){
		if(inputFile1 != NULL) fclose(inputFile1);
		if(inputFile2 != NULL) fclose(inputFile2);
		if(outputFile != NULL) fclose(outputFile);
		return 1;
	}

	// Validação dos arquivos de entrada
	fread(&status, sizeof(char), 1, inputFile1);
	if(status == '0'){
		fclose(inputFile1);
		fclose(inputFile2);
		fclose(outputFile);
		return 2;
	}
	fread(&status, sizeof(char), 1, inputFile2);
	if(status == '0'){
		fclose(inputFile1);
		fclose(inputFile2);
		fclose(outputFile);
		return 2;
	}

	// Leitura dos metadados do cabeçalho do arquivo de entrada 1!
	campoCabecalho cab[5];
	lerCabecalho(cab, 5, inputFile1);
	// Preparando arquivo de saída
	status = '0';
	fwrite(&status, sizeof(char), 1, outputFile);
	// Escrevemos o cabeçalho assumindo que tenho os mesmo metadados do arquivo 1!
	escreverCabecalho(outputFile, cab);

	// Match
	Servidor registro1, registro2;
	registro1.nomeServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	registro2.nomeServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	registro1.cargoServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	registro2.cargoServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
	char temRegistro = 1;
	int aux;

	fseek(inputFile1, TAM_PAGDISCO, SEEK_SET);
	fseek(inputFile2, TAM_PAGDISCO, SEEK_SET);
	fseek(outputFile, TAM_PAGDISCO, SEEK_SET);
	// Leitura do primeiro registro valido de cada arquivo
	// Utiliza auxiliar para resolver caso onde um arquivo não tem registros
	while((aux = lerRegistro(inputFile1, &registro1)) == -1);
	if(aux == 0) temRegistro = 0;
	while((aux = lerRegistro(inputFile2, &registro2)) == -1);
	if(aux == 0) temRegistro = 0;

	// Laço percorrendo os registros
	while(temRegistro){
		if(compararServidor(&registro1, &registro2) > 0){
			// Ler o proximo registro do arquivo 1
			while((aux = lerRegistro(inputFile1, &registro1)) == -1);
			if(aux == 0) temRegistro = 0;
		}
		else if(compararServidor(&registro1, &registro2) < 0){
			// Ler o proximo registro do arquivo 2
			while((aux = lerRegistro(inputFile2, &registro2)) == -1);
			if(aux == 0) temRegistro = 0;
		}
		else {
			// Escreve registro igual na saída
			escreverRegistroAutoFill(&registro1, outputFile);

			// Ler o proximo registro do arquivo 1
			while((aux = lerRegistro(inputFile1, &registro1)) == -1);
			if(aux == 0) temRegistro = 0;
			// Ler o proximo registro do arquivo 2
			while((aux = lerRegistro(inputFile2, &registro2)) == -1);
			if(aux == 0) temRegistro = 0;
		}
	}

	// Atualizando status do arquvio de saída
	status = '1';
	fseek(outputFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, outputFile);

	// Finalização
	free(registro1.nomeServidor);
	free(registro2.nomeServidor);
	free(registro1.cargoServidor);
	free(registro2.cargoServidor);
	fclose(inputFile1);
	fclose(inputFile2);
	fclose(outputFile);
	return 0;
}

// Caso 11
int buscarRegistroInd(const char *inputFileName, const char *inputIndiceName, const char *key){
	indiceNome *indices;
	int tam, numPaginasInd;
	// Carregamento dos indices para memória primária
	indices = carregarArquivoIndices_nome(inputIndiceName, &tam, &numPaginasInd);
	if(indices == NULL){
		return 1;
	}

	// Busca binária nos indices em memória primária
	int pos = buscarIndice_nome(key, indices, tam);
	if(pos < 0){
		free(indices);
		return 3;
	}

	// Inicialização do arquivo de dados
	FILE *dataFile = fopen(inputFileName, "rb");
	if(dataFile == NULL)
		return 1;
	char status = '0';
	fread(&status, sizeof(char), 1, dataFile);
	if(status == '0')
		return 2;

	// Leitura metadados cabeçalho
	campoCabecalho cab[5];
	lerCabecalho(cab, 5, dataFile);
	int numPaginas = 1, lastPage = ftell(dataFile)/TAM_PAGDISCO;

	// Leitura de todos os registros encontrados a partir do arquivo de dados
	Servidor serv;
	serv.nomeServidor = (char *) malloc(MAX_TAM_CAMPO*sizeof(char));
	serv.cargoServidor = (char *) malloc(MAX_TAM_CAMPO*sizeof(char));
	while(strcmp(key, indices[pos].nomeServidor) == 0){
		fseek(dataFile, indices[pos].byteoffset, SEEK_SET);
		if(lastPage != ftell(dataFile)/TAM_PAGDISCO) numPaginas++;
		lastPage = ftell(dataFile)/TAM_PAGDISCO;
		lerRegistro(dataFile, &serv);
		imprimirCamposServidor(&serv, cab);
		pos++;
	}

	printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", numPaginasInd);
	printf("Número de páginas de disco para acessar o arquivo de dados: %d\n", numPaginas);
	
	// Finalizações
	fclose(dataFile);
	free(serv.nomeServidor);
	free(serv.cargoServidor);
	return 0;
}

// Caso 12
int removeRegistroInd(const char *dataFileName, const char *indexFileName, int n){
	// Abertura e testes com o arquivo de dados
	FILE *dataFile = fopen(dataFileName, "r+");
	if(dataFile == NULL)
		return 1;
	char status = '0';
	fread(&status, sizeof(char), 1, dataFile);
	if(status == '0'){
		fclose(dataFile);
		return 2;
	}
	// Escrevemos status do arquivo de dados como 0 pois ele será alterado
	status = '0';
	fseek(dataFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, dataFile);

	// Carregando indices
	indiceNome *indices; int tam, nPag;
	indices = carregarArquivoIndices_nome(indexFileName, &tam, &nPag);
	if(indices == NULL){
		fclose(dataFile);
		return 1;
	}

	char nomeCampo[20], valorCampo[MAX_TAM_CAMPO];
	int i, pos;
	for(i=0; i<n; i++){
		scanf("%s", nomeCampo);
		scan_quote_string(valorCampo);
		if(strcmp(nomeCampo, "nomeServidor") == 0){
			pos = buscarIndice_nome(valorCampo, indices, tam);
			if(pos >= 0){
				while(strcmp(valorCampo, indices[pos].nomeServidor) == 0){
					indiceNome aux = indices[pos];
					// Removendo do arquivo de dados
					fseek(dataFile ,indices[pos].byteoffset, SEEK_SET);
					removerRegistro(dataFile);
					removerIndice_nome(&aux, indices, &tam);
				}
			}
		}
		else
			printf("Remoção solicitada de campo diferente que nomeServidor.\n");
	}

	// Finalização arquivo de dados
	status = '1';
	fseek(dataFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, dataFile);
	fclose(dataFile);

	// Reescrita do arquivo de índices
	FILE *indexFile = fopen(indexFileName, "wb");
	if(indexFile == NULL)
		return -1;
	escreverCabecalhoIndices_nome(indexFile, tam);
	reescreverArquivoIndice_nome(indexFile, tam, indices);
	
	// Finalização arquivo de indices
	status = '1';
	fseek(indexFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, indexFile);
	fclose(indexFile);

	free(indices);
	return 0;
}

// Caso 13
int insereRegistroInd(const char *dataFileName, const char *indexFileName, int n){
	// Abertura e testes com o arquivo de dados
	FILE *dataFile = fopen(dataFileName, "r+");
	if(dataFile == NULL)
		return 1;
	char status = '0';
	fread(&status, sizeof(char), 1, dataFile);
	if(status == '0'){
		fclose(dataFile);
		return 2;
	}
	// Escrevemos status do arquivo de dados como 0 pois ele será alterado
	status = '0';
	fseek(dataFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, dataFile);

	// Carregando indices
	indiceNome *indices; int tam, nPag;
	indices = carregarArquivoIndices_nome(indexFileName, &tam, &nPag);
	if(indices == NULL){
		fclose(dataFile);
		return 1;
	}

	// Verifica se o vetor está cheio ou possui
	// menos espaços livre do que serão feitas inserções
	if(tam%500 == 0 || (500-(tam%500)) < n)
		indices = realloc(indices, sizeof(indiceNome)*(tam+500));

	// Loop de n inserções
	indiceNome novoInd;
	Servidor novoS;
	long aux;
	novoS.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	novoS.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	int i;
	for(i=0; i<n; i++){
		// Leitura do registro a ser inserido a partir da entrada padrão
		scanServidor(&novoS);

		// Chama função de inserção no arquvio de dados que retorna byteoffset de onde foi inserido
		aux = inserirRegistro(dataFile, novoS);
		// Criando indice equivalente
		gerarIndice_nome(&novoInd, novoS, aux);

		// Inserção no vetor de indices
		if(strlen(novoInd.nomeServidor) > 0)
			inserirIndice_nome(novoInd, indices, &tam);
	}
	free(novoS.nomeServidor);
	free(novoS.cargoServidor);

	// Finalização arquivo de dados
	status = '1';
	fseek(dataFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, dataFile);
	fclose(dataFile);

	// Reescrita do arquivo de índices
	FILE *indexFile = fopen(indexFileName, "wb");
	if(indexFile == NULL)
		return -1;
	escreverCabecalhoIndices_nome(indexFile, tam);
	reescreverArquivoIndice_nome(indexFile, tam, indices);
	
	// Finalização arquivo de indices
	status = '1';
	fseek(indexFile, 0, SEEK_SET);
	fwrite(&status, sizeof(char), 1, indexFile);
	fclose(indexFile);

	free(indices);
	return 0;
}

// Caso 14
int comparaBusca(const char *inputFileName, const char *inputIndiceName, char *key){
	// BUSCA SEM AUXÍLIO DE ÍNDICE
	FILE *dataFile;
	Servidor data;
	char status = '0';
	campoCabecalho cab[5];
	int registros = 0;

	dataFile = fopen(inputFileName, "r");
	if(dataFile == NULL){
		return 1;
	}

	fread(&status, sizeof(char), 1, dataFile);
	if(status == '0'){
		fclose(dataFile);
		return 2;
	}

	data.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	data.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);

	// Leitura de metadados com descricao dos campos
	lerCabecalho(cab, 5, dataFile);

	// Posiciona no inicio da primeira pagina de dados do arquivo
	fseek(dataFile, 32000, SEEK_SET);

	// Leitura dos registros
	int isFirst = 1;
	while((lerRegistro(dataFile, &data)) > 0){
		// Valida se o valor do campo do registro eh igual ao valor dado no argumento
		if(testarCampo(&data, "nomeServidor", key)){
			if(isFirst){
				printf("*** Realizando a busca sem o auxílio de índice\n");
				isFirst = 0;
			}
			imprimirCamposServidor(&data, cab);
			registros++;
		}
	}

	// Imprime de acordo com o numero de registros validos
	int numPagNoInd = (ftell(dataFile)/TAM_PAGDISCO)+1;
	int result;
	if(registros == 0) result = 3;
	else printf("Número de páginas de disco acessadas: %d\n", numPagNoInd);

	if(result == 3) {	
		fclose(dataFile);
		free(data.nomeServidor);
		free(data.cargoServidor);
		return 3;
	}

	// BUSCA COM AUXÍLIO DE INDICES
	printf("*** Realizando a busca com o auxílio de um índice secundário fortemente ligado\n");
	indiceNome *indices;
	int tam, numPaginasInd;
	// Carregamento dos indices para memória primária
	indices = carregarArquivoIndices_nome(inputIndiceName, &tam, &numPaginasInd);
	if(indices == NULL){
		return 1;
	}

	// Busca binária nos indices em memória primária
	int pos = buscarIndice_nome(key, indices, tam);
	if(pos < 0){
		free(indices);
		return 3;
	}

	int numPaginasComInd = 1, lastPage = 0;

	// Leitura de todos os registros encontrados a partir do arquivo de dados
	while(strcmp(key, indices[pos].nomeServidor) == 0){
		fseek(dataFile, indices[pos].byteoffset, SEEK_SET);
		if(lastPage != ftell(dataFile)/TAM_PAGDISCO) numPaginasComInd++;
		lastPage = ftell(dataFile)/TAM_PAGDISCO;
		lerRegistro(dataFile, &data);
		imprimirCamposServidor(&data, cab);
		pos++;
	}

	printf("Número de páginas de disco para carregar o arquivo de índice: %d\n", numPaginasInd);
	printf("Número de páginas de disco para acessar o arquivo de dados: %d\n", numPaginasComInd);
	
	printf("\nA diferença no número de páginas de disco acessadas: %d\n", numPagNoInd-numPaginasComInd);
	// Finalizações
	fclose(dataFile);
	free(data.nomeServidor);
	free(data.cargoServidor);
	return 0;
}