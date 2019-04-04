all: orgarq_servidor.o
	gcc -c main.c
	gcc -o a.out main.o orgarq_servidor.o
orgarq_servidor.o: orgarq_servidor.h orgarq_servidor.c
	gcc -c orgarq_servidor.c
run:
	./a.out
test:
	./a.out < in1.txt
clean:
	rm *.o *.out