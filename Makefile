all:
	gcc -c main.c
	gcc -c orgarq_servidor.c
	gcc -c orgarq_cabecalho.c
	gcc -o a.out main.o orgarq_cabecalho.o orgarq_servidor.o
run:
	./a.out
test:
	./a.out < in1.txt
clean:
	rm *.o *.out