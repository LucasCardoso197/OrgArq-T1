#include "orgarq_servidor.h"

void parsearDadosServidor(char *line, Servidor *s){
	char *cptr;

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

int escreverRegistro(Servidor *s, FILE* target){
	char removido = '-', aux;
	long encadeamentoLista = -1;

	// Tamanho da string + '\0' + tag
	int tamanhoNome = strlen(s->nomeServidor) + 2;
	int tamanhoCargo = strlen(s->cargoServidor) + 2;

	// Tamanho do registro levando em conta existencia dos campos de nome e cargo
	int tamanhoRegistro = (s->nomeServidor[0] == '\0') ? 34 : 34 + tamanhoNome + 4;
	tamanhoRegistro = (s->cargoServidor[0] == '\0') ? tamanhoRegistro : tamanhoRegistro + tamanhoCargo + 4;

	// Escrita do registro campo a campo
	fwrite(&removido, sizeof(char), 1, target);
	fwrite(&tamanhoRegistro, sizeof(int), 1, target);
	fwrite(&encadeamentoLista, sizeof(long), 1, target);
	fwrite(&(s->idServidor), sizeof(int), 1, target);
	fwrite(&s->salarioServidor, sizeof(double), 1, target);

	// Espera a formatacao correta do telefone no registro de entrada
	fwrite(s->telefoneServidor, sizeof(char), 14, target); 

	// Escrita condicional de campos de nome
	if(s->nomeServidor[0] != '\0') {
		fwrite(&tamanhoNome, sizeof(int), 1, target);
		aux = 'n';
		fwrite(&aux, sizeof(char), 1, target);
		fwrite(s->nomeServidor, sizeof(char), tamanhoNome-1, target);
	}
	// Escrita condicional de campos de cargo
	if(s->cargoServidor[0] != '\0') {
		fwrite(&tamanhoCargo, sizeof(int), 1, target);
		aux = 'c';
		fwrite(&aux, sizeof(char), 1, target);
		fwrite(s->cargoServidor, sizeof(char), tamanhoCargo-1, target);
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