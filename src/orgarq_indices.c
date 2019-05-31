#include "orgarq_indices.h"

void *gerarVetorIndices(FILE *dataFile, int *n, unsigned long memsize, void (*gerarIndice)(void *, const Servidor, long), int (*fcmp)(const void *, const void *)){
	void *iArray = NULL;
	void *aux = iArray;
	int readReturnValue = 1;
	Servidor s;
    long byteO;
    *n = 0;

    s.cargoServidor = malloc(MAX_TAM_CAMPO*sizeof(char));
    s.nomeServidor = malloc(MAX_TAM_CAMPO*sizeof(char));

    fseek(dataFile, TAM_PAGDISCO, SEEK_SET);
    while(readReturnValue != 0){
        byteO = ftell(dataFile);
		readReturnValue = lerRegistro(dataFile, &s);
		if(readReturnValue > 0){
			if((*n)%500 == 0){
				iArray = realloc(iArray, (*n+500)*memsize);
				aux = iArray+((*n)*memsize);
			}
            gerarIndice(aux, s, byteO);
            aux += memsize;
			(*n)++;
		}
		else if(readReturnValue == -2){
			fprintf(stderr, "Erro no carregamento do arquivo para memória primária.\n");
		}
    }

    free(s.nomeServidor);
    free(s.cargoServidor);

	MS_sort(iArray, (*n), memsize, fcmp);
	return iArray;
}
int escreverArquivoIndices(FILE *outputFile, int n, unsigned long memsize, void *vIndices, int (*escreverIndice)(FILE *, void *indice)){
	int i;
	void *aux = vIndices;
	for(i=0; i<n; i++){
		escreverIndice(outputFile, aux);
		aux += memsize;
	}

	return 0;
}
void *carregarIndices(FILE *inputFile, unsigned long memsize, int n, int (*lerIndice)(FILE *, void *)){
	void *vetor = malloc(memsize * n);
	void *p = vetor;

	int i;
	for(i=0; i<n; i++){
		lerIndice(inputFile, p);
		p += memsize;
	}

	return vetor;
}
int buscarIndice(const void *key, void *iArray, unsigned long memsize, int n, int (*testarChave)(const void *, const void *)){
	int begin, end, mid;
	begin = 0;
	end = n;
	void *indP;

	while(begin <= end){
		mid = (begin+end)/2;
		indP = iArray + memsize*mid;
		if(testarChave(key, indP) < 0){
			end = mid-1;
		}
		else if(testarChave(key, indP) > 0){
			begin = mid+1;
		}
		else
			break;
	}
	if(testarChave(key, indP) != 0)
		return -mid;

	while(mid >= 0 && testarChave(key, indP) == 0){
		mid--;
		indP -= memsize;
	}

	return mid+1;
}
int inserirIndice(void *key, void *ind, void *iArray, unsigned long memsize, int n,
				  int (*testarChave)(const void *, const void *),
				  int (*fcmp)(const void*, const void*)){
	int pos = buscarIndice(key, iArray, memsize, n, testarChave);

}