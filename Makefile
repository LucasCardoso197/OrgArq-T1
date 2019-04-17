all:
	gcc -c main.c -Wall -g
	gcc -c orgarq_servidor.c -Wall -g
	gcc -c orgarq_cabecalho.c -Wall -g
	gcc -o main main.o orgarq_cabecalho.o orgarq_servidor.o -g -Wall
run:
	./main
test1:
	./main < 1.in
test2:
	./main < 2.in
test3:
	./main < 3.in
clean:
	rm *.o
	rm main
	rm *.txt