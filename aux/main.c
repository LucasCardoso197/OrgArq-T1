#include <stdio.h>
#include <stdlib.h>

int main(){
	FILE* fp;
	int number = 1315821;
	int bobo;

	fp = fopen("output.bin", "wb");
	fwrite(&number, sizeof(int), 1, fp);
	fclose(fp);

	fp = fopen("output.bin", "r");
	fread(&bobo, sizeof(int), 1, fp);
	printf("Result: %d\n", bobo);
	fclose(fp);

	return 0;
}