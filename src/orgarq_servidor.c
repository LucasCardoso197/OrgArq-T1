#include "orgarq_servidor.h"

//------------- FUNÇÕES DE ENTRADA E SAÍDA -------------//
void imprimirLinhaServidor(Servidor *s){
	if(s == NULL) {
		fprintf(stderr, "Attempt to print from null pointer.");
		return;
	}
	// ID
	printf("%d ", s->idServidor);
	// Salario
	if(s->salarioServidor == -1)
		printf("         ");
	else
		printf("%.2lf ", s->salarioServidor);
	// Telefone
	if(s->telefoneServidor[0] == '\0')
		printf("              ");
	else
		printf("%.14s", s->telefoneServidor);
	// Nome e cargo
	int tamanhoNome = strlen(s->nomeServidor);
	int tamanhoCargo = strlen(s->cargoServidor);
	if(s->nomeServidor[0] != '\0')
		printf(" %d %s", tamanhoNome, s->nomeServidor);
	if(s->cargoServidor[0] != '\0')
		printf(" %d %s", tamanhoCargo, s->cargoServidor);
	printf("\n");
}

void imprimirCamposServidor(Servidor *s, campoCabecalho *cabecalho){
	if(s == NULL || cabecalho == NULL) {
		fprintf(stderr, "Attempt to print from null pointer.");
		return;
	}
	// ID
	printf("%s: %d\n", cabecalho[0].descricao, s->idServidor);
	// Salario
	if(s->salarioServidor == -1)
		printf("%s: valor nao declarado\n", cabecalho[1].descricao);
	else
		printf("%s: %.2lf\n", cabecalho[1].descricao, s->salarioServidor);
	// Telefone
	if(s->telefoneServidor[0] == '\0')
		printf("%s: valor nao declarado\n", cabecalho[2].descricao);
	else
		printf("%s: %.14s\n", cabecalho[2].descricao, s->telefoneServidor);
	// Nome e cargo
	if(s->nomeServidor[0] == '\0')
		printf("%s: valor nao declarado\n", cabecalho[3].descricao);
	else
		printf("%s: %s\n", cabecalho[3].descricao, s->nomeServidor);
	if(s->cargoServidor[0] == '\0')
		printf("%s: valor nao declarado\n\n", cabecalho[4].descricao);
	else
		printf("%s: %s\n\n", cabecalho[4].descricao, s->cargoServidor);
}

void parsearDadosServidor(char *line, Servidor *s){
	char *cptr;
	int i;

	// Garantir inicializacao correta dos campos caso encontre valores nulos
	resetarServidor(s);

	// Lendo ID
	s->idServidor = (int) strtol(line, &cptr, 10);
	cptr++; // Pula ','
	// Lendo salario
	s->salarioServidor = (double) strtod(cptr, &cptr);
	cptr++;

	// Lendo telefone
	if(sscanf(cptr, "%[^,]", s->telefoneServidor) == 0){
		cptr++;
	}
	else {
		cptr += 15;
	}
	// Lendo nome
	if(sscanf(cptr, "%[^,]", s->nomeServidor) == 0){
		cptr++;
	}
	else {
		cptr = cptr + strlen(s->nomeServidor) + 1;
	}
	// Lendo nome ateh encontrar tabulacao vertical ou caractere nova linha
	while((*cptr != 13 && *cptr != 0) && *cptr != 10){
		s->cargoServidor[i] = *cptr;
		i++;
		cptr++;
	}
	s->cargoServidor[i] = '\0';
}

void scanServidor(Servidor *s){
	char argAux[MAX_TAM_CAMPO];
	resetarServidor(s);
	int i;

	scanf("%d ", &s->idServidor);
	scanf("%s ", argAux);
	if(strcmp(argAux, "NULO") != 0)
		s->salarioServidor = strtod(argAux, NULL);
	scan_quote_string(s->telefoneServidor);
	if(s->telefoneServidor[0] == '\0')
		for(i=1; i<14; i++)
			s->telefoneServidor[i] = '@';
	scan_quote_string(s->nomeServidor);
	scan_quote_string(s->cargoServidor);
}


//------------- FUNÇÕES AUXILIARES -------------//
void resetarServidor(Servidor *s){
	int i;
	s->telefoneServidor[0] = '\0';
	// Preenchendo lixo como '@'
	for(i = 1; i < 14; i++)
		s->telefoneServidor[i] = '@';
	s->nomeServidor[0] = '\0';
	s->cargoServidor[0] = '\0';
	s->idServidor = 0;
	s->salarioServidor = -1.0f;
}

void copiarServidor(Servidor *target, Servidor *s){
	int i;
	target->idServidor = s->idServidor;
	target->salarioServidor = s->salarioServidor;
	for(i=0; i<14; i++)
		target->telefoneServidor[i] = s->telefoneServidor[i];
	strcpy(target->nomeServidor, s->nomeServidor);
	strcpy(target->cargoServidor, s->cargoServidor);
}

int tamanhoRegServidor(Servidor *s){
	int tamanhoNome = strlen(s->nomeServidor) + 2;
	int tamanhoCargo = strlen(s->cargoServidor) + 2;
	int tamanhoRegistro = (s->nomeServidor[0] == '\0') ? 34 : 34 + tamanhoNome + 4;
	tamanhoRegistro = (s->cargoServidor[0] == '\0') ? tamanhoRegistro : tamanhoRegistro + tamanhoCargo + 4;
	return tamanhoRegistro;
}

int testarCampo(Servidor *s, char *nomeCampo, char *argumento){
	// Primeiro testa-se qual campo esta sendo comparado
	if(!strcmp(nomeCampo, "idServidor")){
		int auxi;
		// Entao retira do argumento a informacao a ser comparada
		sscanf(argumento, "%d", &auxi);
		if(auxi == s->idServidor)
			return 1;
	}
	else if (!strcmp(nomeCampo, "salarioServidor")){
		double auxd;
		sscanf(argumento, "%lf", &auxd);
		if(auxd == s->salarioServidor)
			return 1;
	}
	else if (!strcmp(nomeCampo, "telefoneServidor")){
		if(!strncmp(argumento, s->telefoneServidor, 14))
			return 1;
	}
	else if (!strcmp(nomeCampo, "nomeServidor")){
		if(!strcmp(argumento, s->nomeServidor))
			return 1;
	}
	else if (!strcmp(nomeCampo, "cargoServidor")){
		if(!strcmp(argumento, s->cargoServidor))
			return 1;
	}
	return 0;
}


//------------- FUNÇÕES DE INTERAÇÃO COM ARQUIVOS -------------//
int escreverRegistro(Servidor *s, FILE* targetFile, int extra){
	char removido = '-', trash = '@', aux;
	long encadeamentoLista = -1;
	int i;

	// Tamanho da string + '\0' + tag
	int tamanhoNome = strlen(s->nomeServidor) + 2;
	int tamanhoCargo = strlen(s->cargoServidor) + 2;

	// Tamanho do registro levando em conta existencia dos campos de nome e cargo
	int tamanhoRegistro = (s->nomeServidor[0] == '\0') ? 34 : 34 + tamanhoNome + 4;
	tamanhoRegistro = (s->cargoServidor[0] == '\0') ? tamanhoRegistro : tamanhoRegistro + tamanhoCargo + 4;
	tamanhoRegistro += extra;

	// Escrita do registro campo a campo
	fwrite(&removido, sizeof(char), 1, targetFile);
	fwrite(&tamanhoRegistro, sizeof(int), 1, targetFile);
	fwrite(&encadeamentoLista, sizeof(long), 1, targetFile);
	fwrite(&(s->idServidor), sizeof(int), 1, targetFile);
	fwrite(&s->salarioServidor, sizeof(double), 1, targetFile);

	// Espera a formatacao correta do telefone no registro de entrada
	fwrite(s->telefoneServidor, sizeof(char), 14, targetFile); 

	// Escrita condicional de campos de nome
	if(s->nomeServidor[0] != '\0') {
		fwrite(&tamanhoNome, sizeof(int), 1, targetFile);
		aux = 'n';
		fwrite(&aux, sizeof(char), 1, targetFile);
		fwrite(s->nomeServidor, sizeof(char), tamanhoNome-1, targetFile);
	}
	// Escrita condicional de campos de cargo
	if(s->cargoServidor[0] != '\0') {
		fwrite(&tamanhoCargo, sizeof(int), 1, targetFile);
		aux = 'c';
		fwrite(&aux, sizeof(char), 1, targetFile);
		fwrite(s->cargoServidor, sizeof(char), tamanhoCargo-1, targetFile);
	}

	// Escreve @ como lixo no final do registro, de acordo com o argumento de entrada
	for(i=0; i < extra; i++){
		fwrite(&trash, sizeof(char), 1, targetFile);
	}

	// Tamanho do registro + byte de remocao + inteiro indicador de tamanho de registro
	return tamanhoRegistro + 5;
}

int escreverRegistroAutoFill(Servidor *s, FILE* targetFile){
	char removido = '-', aux;
	long encadeamentoLista = -1;

	// Tamanho da string + '\0' + tag
	int tamanhoNome = strlen(s->nomeServidor) + 2;
	int tamanhoCargo = strlen(s->cargoServidor) + 2;
	int tamanhoRegistro = tamanhoRegServidor(s);

	// Se o registro nao couber na pagina preenchemos ela
	long posicaoAtual = ftell(targetFile)%TAM_PAGDISCO;
	if(posicaoAtual+tamanhoRegistro+5 > TAM_PAGDISCO){
		preencherPaginaDeDisco(targetFile);
	}

	// Escrita do registro campo a campo
	fwrite(&removido, sizeof(char), 1, targetFile);
	fwrite(&tamanhoRegistro, sizeof(int), 1, targetFile);
	fwrite(&encadeamentoLista, sizeof(long), 1, targetFile);
	fwrite(&(s->idServidor), sizeof(int), 1, targetFile);
	fwrite(&s->salarioServidor, sizeof(double), 1, targetFile);

	// Espera a formatacao correta do telefone no registro de entrada
	fwrite(s->telefoneServidor, sizeof(char), 14, targetFile); 

	// Escrita condicional de campos de nome
	if(s->nomeServidor[0] != '\0') {
		fwrite(&tamanhoNome, sizeof(int), 1, targetFile);
		aux = 'n';
		fwrite(&aux, sizeof(char), 1, targetFile);
		fwrite(s->nomeServidor, sizeof(char), tamanhoNome-1, targetFile);
	}
	// Escrita condicional de campos de cargo
	if(s->cargoServidor[0] != '\0') {
		fwrite(&tamanhoCargo, sizeof(int), 1, targetFile);
		aux = 'c';
		fwrite(&aux, sizeof(char), 1, targetFile);
		fwrite(s->cargoServidor, sizeof(char), tamanhoCargo-1, targetFile);
	}

	// Tamanho do registro + byte de remocao + inteiro indicador de tamanho de registro
	return tamanhoRegistro + 5;
}

long buscarRegistro(FILE *inputFile, char *nomeCampo, char *argumento){
	Servidor data;
	int bytesLidos;
	long result = -1;

	data.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	data.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);

	// Leitura sequencial
	while((bytesLidos = lerRegistro(inputFile, &data)) != 0){
		// Valida se o valor do campo do registro eh igual ao valor dado no argumento
		if(testarCampo(&data, nomeCampo, argumento) && bytesLidos != -1){
			result = ftell(inputFile) - bytesLidos;
			break;
		}
	}

	free(data.nomeServidor);
	free(data.cargoServidor);

	return result;
}

int lerRegistro(FILE *inputFile, Servidor *s){
	char *buffer, *cptr, caux;
	int aux, tamanhoRegistro;
	resetarServidor(s);

	// Leitura caso nao EOF
	if(fread(&caux, sizeof(char), 1, inputFile) == 0) return 0;

	// Leitura de fato do registro
	fread(&tamanhoRegistro, sizeof(int), 1, inputFile);
	if(tamanhoRegistro < 0){
		fprintf(stderr, "Critical error, read unexpected value from file.\n");
		return -2;
	}

	// Registro que esta tentando ler foi apagado
	if(caux == '*') {
		fseek(inputFile, tamanhoRegistro, SEEK_CUR);
		return -1;
	}

	buffer = (char*) malloc(sizeof(char)*tamanhoRegistro);
	fread(buffer, sizeof(char), tamanhoRegistro, inputFile);
	// Le o registro inteiro em um buffer de memoria, para interpretacao dos dados.
	memcpy(&s->idServidor, buffer+8, 4);
	memcpy(&s->salarioServidor, buffer+12, 8);
	memcpy(s->telefoneServidor, buffer+20, 14);
	if(tamanhoRegistro == 34) { // Nome nulo, cargo nulo e sem bytes extra
		free(buffer);
		return tamanhoRegistro+5;
	}
	memcpy(&caux, buffer+34, 1);
	if(caux == '@'){ // Nome nulo, cargo nulo com pelo menos 1 byte extra
		free(buffer);
		return tamanhoRegistro+5;
	}
	memcpy(&aux, buffer+34, 4);
	memcpy(&caux, buffer+38, 1);
	if(caux == 'n') {
		memcpy(s->nomeServidor, buffer+39, aux-1);

		if((tamanhoRegistro == 38+aux) || (buffer[38+aux] == '@')) {
			// Nome nao nulo, cargo nulo com ou sem bytes extra
			free(buffer);
			return tamanhoRegistro+5;
		}
		// Nome nao nulo, cargo nao nulo com ou sem bytes extra
		cptr = buffer+38+aux;
		memcpy(&aux, cptr, 4);
		memcpy(&caux, cptr+4, 1);
	}
	// Nome nulo, cargo nao nulo com ou seu bytes extra
	else if(caux == 'c'){
		memcpy(s->cargoServidor, buffer+39, aux-1);
		free(buffer);
		return tamanhoRegistro+5;
	}
	// Ambos nomes e cargo nao nulos
	memcpy(s->cargoServidor, cptr+5, aux-1);
	cptr = NULL;
	free(buffer);
	return tamanhoRegistro+5;
}

long buscaProxRegistro(FILE *inputFile){
	char caux;
	int tamanhoRegistro;
	if(fread(&caux, sizeof(char), 1, inputFile) == 0) return 0;

	// Leitura do tamanho do registro
	fread(&tamanhoRegistro, sizeof(int), 1, inputFile);
	if(tamanhoRegistro < 0){
		fprintf(stderr, "Critical error, read unexpected value from file.\n");
		return -1;
	}

	fseek(inputFile, tamanhoRegistro, SEEK_CUR);
	return tamanhoRegistro+5;
}

int removerRegistro(FILE *inputFile){
	char caux;
	long pos;
	int i, tam;
	
	if(inputFile == NULL) return 1;

	pos = ftell(inputFile);
	if(fread(&caux, sizeof(char), 1, inputFile) == 0) return 2;
	if(caux == '*') return 3;

	fread(&tam, sizeof(int), 1, inputFile);

	// Apagando
	fseek(inputFile, 8, SEEK_CUR);
	caux = '@';
	for(i=8; i<tam; i++)
		fwrite(&caux, sizeof(char), 1, inputFile);
	fseek(inputFile, pos, SEEK_SET);
	caux = '*';
	fwrite(&caux, sizeof(char), 1, inputFile);
	fseek(inputFile, tam+4, SEEK_CUR);

	// Insercao lista
	inserirLista(inputFile, pos, tam);

	return 0;
}

int inserirRegistro(FILE *outputFile, Servidor s){
	int extra, tam = tamanhoRegServidor(&s);
	long enderecoInsercao, aux;

	enderecoInsercao = removerLista(outputFile, tam);
	if(enderecoInsercao == -1){
		fseek(outputFile, 0, SEEK_END);
		aux = ftell(outputFile)%TAM_PAGDISCO;
		// Se a insercao do novo registro irá ocupar a proxima pagina
		// de disco, preenchemos a atual para nao dividir o registro
		if(aux+tam+5 > TAM_PAGDISCO){
			preencherPaginaDeDisco(outputFile);
		}
	
		escreverRegistro(&s, outputFile, 0);
	}
	else{
		fseek(outputFile, enderecoInsercao+offset_tam_registro, SEEK_SET);
		fread(&extra, sizeof(int), 1, outputFile);
		fseek(outputFile, enderecoInsercao, SEEK_SET);
		escreverRegistro(&s, outputFile, extra-tam);
	}

	return 0;
}


void preencherPaginaDeDisco(FILE *fp){
	// Encontrando o ultimo registro do arquivo para preenchimento
	// Busca apenas a partir do começo da ultima pagina de disco
	int lastSize, i;
	long aux = ftell(fp)/TAM_PAGDISCO;
	fseek(fp, aux*TAM_PAGDISCO, SEEK_SET);
	while((aux = buscaProxRegistro(fp)) > 0)
		lastSize = aux;
	
	// Preenchimento
	aux = ftell(fp)%TAM_PAGDISCO; // Quantos bytes faltam para preencher a pagina
	fseek(fp, -(lastSize-1), SEEK_CUR);
	lastSize += TAM_PAGDISCO-aux-5;
	fwrite(&lastSize, sizeof(int), 1, fp);
	fseek(fp, 0, SEEK_END);
	char trash = '@';
	for(i = 0; i < TAM_PAGDISCO-aux; i++)
		fwrite(&trash, sizeof(char), 1, fp);
	// SAI DA FUNÇÃO COM PONTEIRO DE ARQUIVO NO FINAL DO ARQUIVO
}

int atualizarRegistro(FILE *updateFile, char *campoAtualiza, char *argAtualiza, Servidor *s){
	Servidor novo;
	novo.cargoServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);
	novo.nomeServidor = (char *)malloc(sizeof(char)*MAX_TAM_CAMPO);

	// Criando registro de servidor atualizado
	copiarServidor(&novo, s);
	if(!strcmp(campoAtualiza, "idServidor")){
		int id = atoi(argAtualiza);
		novo.idServidor = id;
	}
	else if (!strcmp(campoAtualiza, "salarioServidor")){
		double sal = atof(argAtualiza);
		novo.salarioServidor = sal;
	}
	else if (!strcmp(campoAtualiza, "telefoneServidor")){
		int i;
		for(i=0; i<14; i++)
			novo.telefoneServidor[i] = argAtualiza[i];
	}
	else if (!strcmp(campoAtualiza, "nomeServidor")){
		strcpy(novo.nomeServidor, argAtualiza);
	}
	else if (!strcmp(campoAtualiza, "cargoServidor")){
		strcpy(novo.cargoServidor, argAtualiza);
	}

	// Teste para definir tipo de atualização
	int tamNovo = tamanhoRegServidor(&novo);
	int tamAnt = tamanhoRegServidor(s);
	if(tamNovo <= tamAnt){
		// Atualização do registro in-place
		escreverRegistro(&novo, updateFile, tamAnt-tamNovo);
	}
	else {
		// Remoção do registro para inserção do atualizado
		removerRegistro(updateFile);
		long currentPos = ftell(updateFile);
		inserirRegistro(updateFile, novo);
		fseek(updateFile, currentPos, SEEK_SET);
	}

	free(novo.nomeServidor);
	free(novo.cargoServidor);
	return 0;
}

Servidor *carregarArquivoDados(FILE *inputFile, int *tam){
	int readReturnValue = 1;
	*tam = 0;
	Servidor *sp, *sArray = NULL, registroLido;

	// Alocacao inicial
	registroLido.nomeServidor = (char *)malloc(MAX_TAM_CAMPO*sizeof(char));
	registroLido.cargoServidor = (char *)malloc(MAX_TAM_CAMPO*sizeof(char));

	fseek(inputFile, TAM_PAGDISCO, SEEK_SET);
	// Loop de leitura
	while(readReturnValue != 0){
		readReturnValue = lerRegistro(inputFile, &registroLido);
		if(readReturnValue > 0){
			if((*tam)%500 == 0){
				sArray = (Servidor *) realloc(sArray, (*tam+500)*sizeof(Servidor));
				sp = &sArray[*tam];
			}
			sp->nomeServidor = (char *)malloc(MAX_TAM_CAMPO*sizeof(char));
			sp->cargoServidor = (char *)malloc(MAX_TAM_CAMPO*sizeof(char));
			copiarServidor(sp, &registroLido);
			sp++;
			(*tam)++;
		}
		else if(readReturnValue == -2){
			fprintf(stderr, "Erro no carregamento do arquivo para memória primária.\n");
		}
	}

	free(registroLido.nomeServidor);
	free(registroLido.cargoServidor);
	return sArray;
}

void liberarVetorServidor(Servidor *vetor, int tam){
	int i;
	Servidor *sp = vetor;
	for(i=0; i<tam; i++, sp++){
		if(sp->nomeServidor != NULL)
			free(sp->nomeServidor);
		if(sp->cargoServidor != NULL)
			free(sp->cargoServidor);
	}
	free(vetor);
}

int compararServidor(const void *s1, const void *s2){
	Servidor *s1p, *s2p;
	s1p = (Servidor *)s1;
	s2p = (Servidor *)s2;
	return (s2p->idServidor-s1p->idServidor);
}