all:
	gcc -c main.c -Wall
	gcc -c ./src/orgarq_servidor.c -Wall
	gcc -c ./src/orgarq_cabecalho.c -Wall
	gcc -c ./src/orgarq_listaRemocao.c -Wall
	gcc -c ./src/utils.c -Wall
	gcc -o main *.o -Wall
run:
	./main
test1:
	./main < ./_input/1.in
test2:
	./main < ./_input/2.in
test3:
	./main < ./_input/3.in
test4:
	./main < ./_input/1.in
	./main < ./_input/4.in
test5:
	./main < ./_input/1.in
	./main < ./_input/4.in
	./main < ./_input/5.in
test6:
	./main < ./_input/1.in
	./main < ./_input/6.in
dump:
	hexdump -Cv arquivoTrab1.bin > ./_output/out.txt
clean:
	rm *.o
	rm main
	rm *.txt