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
	// Reading name
	sscanf(cptr, "%[^,\n]", s->nomeServidor);
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
	s->telefoneServidor[0] = '\0';
	s->nomeServidor[0] = '\0';
	s->cargoServidor[0] = '\0';
	s->idServidor = 0;
	s->salarioServidor = 0.0f;
}