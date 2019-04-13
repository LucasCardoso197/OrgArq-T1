all:
	gcc -c main.c -Wall -g
	gcc -c orgarq_servidor.c -Wall -g
	gcc -c orgarq_cabecalho.c -Wall -g
	gcc -o main main.o orgarq_cabecalho.o orgarq_servidor.o -g -Wall
run:
	./main
test1:
	./main < in1.txt
test2:
	./main < in2.txt
test3:
	./main < in3.txt
clean:
	rm *.o
	rm main
