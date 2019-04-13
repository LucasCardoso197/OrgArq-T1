#include "orgarq_servidor.h"

void imprimirLinhaServidor(Servidor *s){
	if(s == NULL) {
		fprintf(stderr, "Attempt to print from null pointer.");
		return;
	}
	printf("%d %.2lf %.14s ", s->idServidor, s->salarioServidor, s->telefoneServidor);
	int tamanhoNome = strlen(s->nomeServidor);
	int tamanhoCargo = strlen(s->cargoServidor);
	if(s->nomeServidor[0] != '\0')
		printf("%d %s ", tamanhoNome, s->nomeServidor);
	if(s->cargoServidor[0] != '\0')
		printf("%d %s", tamanhoCargo, s->cargoServidor);
	printf("\n");
}

void imprimirCamposServidor(Servidor *s){
	if(s == NULL) {
		fprintf(stderr, "Attempt to print from null pointer.");
		return;
	}
	printf("numero de identificacao do servidor: %d\n", s->idServidor);
	printf("salario do servidor: %.2lf\n", s->salarioServidor);
	printf("telefone celular do servidor: %.14s\n", s->telefoneServidor);
	printf("nome do servidor: %s\n", s->nomeServidor);
	printf("cargo do servidor: %s\n\n", s->cargoServidor);
}

void resetarServidor(Servidor *s){
	int i;
	s->telefoneServidor[0] = '\0';
	for(i = 1; i < 14; i++)
		s->telefoneServidor[i] = '@';
	s->nomeServidor[0] = '\0';
	s->cargoServidor[0] = '\0';
	s->idServidor = 0;
	s->salarioServidor = 0.0f;
}

void parsearDadosServidor(char *line, Servidor *s){
	char *cptr;
	int i;

	// Garantir inicializacao correta dos campos caso encontre valores nulos
	resetarServidor(s);

	// Reading ID
	s->idServidor = (int) strtol(line, &cptr, 10);
	cptr++; // Skips ','
	// Reading salary
	s->salarioServidor = (double) strtod(cptr, &cptr);
	cptr++;

	// Reading phone
	if(sscanf(cptr, "%[^,]", s->telefoneServidor) == 0){
		cptr++;
	}
	else {
		cptr += 15;
	}
	// Reading role
	if(sscanf(cptr, "%[^,]", s->nomeServidor) == 0){
		cptr++;
	}
	else {
		cptr = cptr + strlen(s->nomeServidor) + 1;
	}
	// Reading name if line isn't over or found the 'mysterious' vertical tab
	while((*cptr != 13 && *cptr != 0) && *cptr != 10){
		s->cargoServidor[i] = *cptr;
		i++;
		cptr++;
	}
	s->cargoServidor[i] = '\0';
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

	for(i=0; i < extra; i++){
		fwrite(&trash, sizeof(char), 1, targetFile);
	}

	// Tamanho do registro + byte de remocao + inteiro indicador de tamanho de registro
	return tamanhoRegistro + 5;
}

int tamanhoRegServidor(Servidor *s){
	int tamanhoNome = strlen(s->nomeServidor) + 2;
	int tamanhoCargo = strlen(s->cargoServidor) + 2;
	int tamanhoRegistro = (s->nomeServidor[0] == '\0') ? 34 : 34 + tamanhoNome + 4;
	tamanhoRegistro = (s->cargoServidor[0] == '\0') ? tamanhoRegistro : tamanhoRegistro + tamanhoCargo + 4;
	return tamanhoRegistro;
}

int lerRegistro(FILE *inputFile, Servidor *s){
	char *buffer, *cptr, caux;
	int aux, tamanhoRegistro;
	resetarServidor(s);

	// Leitura
	fread(&caux, sizeof(char), 1, inputFile);
	fread(&tamanhoRegistro, sizeof(int), 1, inputFile);
	if(feof(inputFile) || ferror(inputFile)) return -1;
	if(tamanhoRegistro < 0){
		fprintf(stderr, "Critical error, read unexpected value from file.\n");
		return -2;
	}
	buffer = (char*) malloc(sizeof(char)*tamanhoRegistro);
	fread(buffer, sizeof(char), tamanhoRegistro, inputFile);
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
	memcpy(s->cargoServidor, cptr+5, aux-1);
	cptr = NULL;
	free(buffer);
	return tamanhoRegistro+5;
}