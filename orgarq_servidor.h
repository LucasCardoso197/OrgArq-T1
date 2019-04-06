#ifndef _ORGARQ_SERVIDOR_H
#define _ORGARQ_SERVIDOR_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
	int idServidor;
	double salarioServidor;
	char telefoneServidor[14], *nomeServidor, *cargoServidor;
} Servidor;

void parsearDadosServidor(char *line, Servidor *s);
void imprimirServidor(Servidor *s);
void resetarServidor(Servidor *s);
int tamanhoRegServidor(Servidor *s);
int escreverRegistro(Servidor *s, FILE* target);

#endif