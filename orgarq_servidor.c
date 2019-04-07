#include "orgarq_servidor.h"

void imprimirServidor(Servidor *s){
	printf("|_Servidor_|\n");
	printf("ID: %d\n", s->idServidor);
	printf("Telefone: %s\n", s->telefoneServidor);
	printf("Salario: %.2lf\n", s->salarioServidor);
	printf("Cargo: %s\n", s->cargoServidor);
	printf("Nome: %s\n", s->nomeServidor);
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

	// Garantir inicializacao correta dos campos caso encontre valores nulos
	resetarServidor(s);

	// Reading ID
	s->idServidor = (int) strtol(line, &cptr, 10);
	cptr++;
	// Reading phone
	if(sscanf(cptr, "%[^,]", s->telefoneServidor) == 0){
		cptr++;
	}
	else {
		cptr += 15;
	}
	// Reading salary
	s->salarioServidor = (double) strtod(cptr, &cptr);
	cptr++;
	// Reading role
	if(sscanf(cptr, "%[^,]", s->cargoServidor) == 0){
		cptr++;
	}
	else {
		cptr = cptr + strlen(s->cargoServidor) + 1;
	}
	// Reading name if line isn't over or found the 'mysterious' vertical tab
	int i = 0;
	while(*cptr != 13 && *cptr != 0){
		s->nomeServidor[i] = *cptr;
		i++;
		cptr++;
	}
	s->nomeServidor[i] = '\0';
}

void copiarServidor(Servidor *target, Servidor *s){
	target->idServidor = s->idServidor;
	target->salarioServidor = s->salarioServidor;
	strcpy(target->telefoneServidor, s->telefoneServidor);
	strcpy(target->nomeServidor, s->nomeServidor);
	strcpy(target->cargoServidor, s->cargoServidor);
}

int escreverRegistro(Servidor *s, FILE* targetFile, int extra){
	char removido = '-', trash = '@', aux;
	long encadeamentoLista = -1;
	int currentPosition = ftell(targetFile);
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
		printf("%d - Filling at...\n", i);
		imprimirServidor(s);
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

size_t lerRegistro(FILE *inputFile, Servidor *s){
	char *buffer, *cptr, caux;
	int i, aux, tamanhoRegistro;
	resetarServidor(s);

	// Leitura
	fseek(inputFile, 1, SEEK_CUR);
	fread(&tamanhoRegistro, sizeof(int), 1, inputFile);
	buffer = malloc(tamanhoRegistro);
	fread(buffer, sizeof(char), tamanhoRegistro, inputFile);
	memcpy(&s->idServidor, buffer+8, 4);
	memcpy(&s->salarioServidor, buffer+12, 8);
	memcpy(s->telefoneServidor, buffer+20, 14);
	if(tamanhoRegistro == 34) {
		free(buffer);
		return 0;
	}
	memcpy(&aux, buffer+34, 4);
	memcpy(&caux, buffer+38, 1);
	if(caux == 'n') {
		memcpy(s->nomeServidor, buffer+39, aux-1);
		if((tamanhoRegistro == 38+aux) || (buffer[38+aux] == '@')) {
			free(buffer);
			return 0;
		}
		cptr = buffer+38+aux;
		memcpy(&aux, cptr, 4);
		memcpy(&caux, cptr+4, 1);
	}
	else if(caux == 'c'){
		memcpy(s->cargoServidor, buffer+39, aux-1);
		free(buffer);
		return 0;
	}
	else { //caux == '@'
		free(buffer);
		return 0;
	}
	memcpy(s->cargoServidor, cptr+5, aux-1);
	free(buffer);
	return 0;
}