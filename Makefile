all:
	gcc -c main.c -Wall
	gcc -c ./src/orgarq_servidor.c -Wall
	gcc -c ./src/orgarq_cabecalho.c -Wall
	gcc -c ./src/orgarq_listaRemocao.c -Wall
	gcc -c ./src/utils.c -Wall
	gcc -c ./src/merge_sort.c -Wall
	gcc -o programaTrab3 *.o -Wall
run:
	./programaTrab3
test1:
	./programaTrab3 < ./_input/1.in
test2:
	./programaTrab3 < ./_input/2.in
test3:
	./programaTrab3 < ./_input/3.in
test4:
	./programaTrab3 < ./_input/1.in
	./programaTrab3 < ./_input/4.in
test5:
	./programaTrab3 < ./_input/1.in
	./programaTrab3 < ./_input/4.in
	./programaTrab3 < ./_input/5.in
test6:
	./programaTrab3 < ./_input/1.in
	./programaTrab3 < ./_input/6.in
test7:
	./programaTrab3 < ./_input/1.in
	./programaTrab3 < ./_input/7.in
test8:
	./programaTrab3 < ./_input/8.in
test9:
	./programaTrab3 < ./_input/9.in
testCustom:
	./programaTrab3 < ./_input/custom.in
dump:
	hexdump -Cv arquivoTrab1.bin > ./_output/out.txt
clean:
	rm *.o
	rm programaTrab3
	rm *.txt