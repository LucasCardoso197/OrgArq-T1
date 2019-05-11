#include "orgarq_listaRemocao.h"

int buscarPosLista(FILE *arq, int tam, long *prev, long *next){
	long prevPos = -1, nextPos;
	int nextTam;

	// Leitura topo lista
	fseek(arq, offset_topo_lista, SEEK_SET);
	fread(&nextPos, sizeof(long), 1, arq);

	// Lista vazia
	if(nextPos == -1) {
		*next = -1;
		*prev = -1;
		return 0;
	}
	fseek(arq, nextPos+offset_tam_registro, SEEK_SET);
	fread(&nextTam, sizeof(int), 1, arq);

	// Encontrando posicao
	while(tam > nextTam && nextPos != -1) {
		// Prev = Next
		prevPos = nextPos;

		// Next = ler(Prev)
		fseek(arq, prevPos+offset_encadeamento_lista, SEEK_SET);
		fread(&nextPos, sizeof(long), 1, arq);
		if(nextPos != -1){
			fseek(arq, nextPos+offset_tam_registro, SEEK_SET);
			fread(&nextTam, sizeof(int), 1, arq);
		}
	}
	// Sai no comeco da lista
	if(prevPos == -1){
		*next = nextPos;
		*prev = -1;
		// Tam buscado nao cabe no comeco
		if(tam < nextTam){
			return 2;
		}
		// Tam buscado cabe no comeco
		else
			return 1;
	}
	// Tam maior do que o ultimo elemento da lista
	if(tam > nextTam && nextPos == -1){
		*next = -1;
		*prev = -1;
		return 4;
	}
	*next = nextPos;
	*prev = prevPos;
	return 3;
}

int buscarPosListaEstavel(FILE *arq, int tam, long *prev, long *next){
	long prevPos = -1, nextPos;
	int nextTam;

	// Leitura topo lista
	fseek(arq, offset_topo_lista, SEEK_SET);
	fread(&nextPos, sizeof(long), 1, arq);

	// Lista vazia
	if(nextPos == -1) {
		*next = -1;
		*prev = -1;
		return 0;
	}
	fseek(arq, nextPos+offset_tam_registro, SEEK_SET);
	fread(&nextTam, sizeof(int), 1, arq);

	// Encontrando posicao
	while(tam >= nextTam && nextPos != -1) {
		// Prev = Next
		prevPos = nextPos;

		// Next = ler(Prev)
		fseek(arq, prevPos+offset_encadeamento_lista, SEEK_SET);
		fread(&nextPos, sizeof(long), 1, arq);
		if(nextPos != -1){
			fseek(arq, nextPos+offset_tam_registro, SEEK_SET);
			fread(&nextTam, sizeof(int), 1, arq);
		}
	}
	// Sai no comeco da lista
	if(prevPos == -1){
		*next = nextPos;
		*prev = -1;
		// Tam buscado nao cabe no comeco
		if(tam < nextTam){
			return 2;
		}
		// Tam buscado cabe no comeco
		else
			return 1;
	}
	// Tam maior do que o ultimo elemento da lista
	if(tam > nextTam && nextPos == -1){
		*next = -1;
		*prev = -1;
		return 4;
	}
	
	*next = nextPos;
	*prev = prevPos;
	return 3;
}

int inserirLista(FILE *arq, long pos, int tam){
	long prevPos, nextPos;
	long startPos = ftell(arq);

	buscarPosListaEstavel(arq, tam, &prevPos, &nextPos);

	// Insere começo da lista
	if (prevPos == -1){
		// topo = novo
		fseek(arq, offset_topo_lista, SEEK_SET);
		fwrite(&pos, sizeof(long), 1, arq);
		// novo->prox = topo
		fseek(arq, pos+offset_encadeamento_lista, SEEK_SET);
		fwrite(&nextPos, sizeof(long), 1, arq);
	}
	// Insere final e meio da lista
	else {
		// prev->prox = novo
		fseek(arq, prevPos+offset_encadeamento_lista, SEEK_SET);
		fwrite(&pos, sizeof(long), 1, arq);
		// novo->prox = prev->prox
		fseek(arq, pos+offset_encadeamento_lista, SEEK_SET);
		fwrite(&nextPos, sizeof(long), 1, arq);
	}

	fseek(arq, startPos, SEEK_SET);
	return 0;
}

long removerLista(FILE *arq, int tam){
	long prevPos, nextPos, aux;
	long startPos = ftell(arq);
	int listState;

	listState = buscarPosLista(arq, tam, &prevPos, &nextPos);
	// Remover começo lista
	if(listState == 1){
		// topo = prev->prox
		fseek(arq, nextPos+offset_encadeamento_lista, SEEK_SET);
		fread(&aux, sizeof(long), 1, arq);
		fseek(arq, offset_topo_lista, SEEK_SET);
		fwrite(&aux, sizeof(long), 1, arq);
	}
	// Remover meio ou final da lista
	else if(listState == 3){
		// prev->prox = next->prox
		fseek(arq, nextPos+offset_encadeamento_lista, SEEK_SET);
		fread(&aux, sizeof(long), 1, arq);
		fseek(arq, prevPos+offset_encadeamento_lista, SEEK_SET);
		fwrite(&aux, sizeof(long), 1, arq);
	}
	else{
		//fprintf(stderr, "Não foi possível realizar a remoção de %x\n", tam);
		fseek(arq, startPos, SEEK_SET);
		return -1;
	}

	fseek(arq, startPos, SEEK_SET);
	return nextPos;
}

/*
prev(size<s) -> next(size>s)
Busca retorna pos de prev e next

*/