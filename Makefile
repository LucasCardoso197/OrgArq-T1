all:
	gcc -c main.c -Wall
	gcc -c ./src/orgarq_servidor.c -Wall
	gcc -c ./src/orgarq_cabecalho.c -Wall
	gcc -c ./src/orgarq_listaRemocao.c -Wall
	gcc -c ./src/orgarq_indices.c -Wall
	gcc -c ./src/orgarq_indice_nome.c -Wall
	gcc -c ./src/utils.c -Wall
	gcc -c ./src/merge_sort.c -Wall
	gcc -o programaTrab4 *.o -Wall
run:
	./programaTrab4
test1:
	./programaTrab4 < ./_input/1.in
test2:
	./programaTrab4 < ./_input/2.in
test3:
	./programaTrab4 < ./_input/3.in
test4:
	./programaTrab4 < ./_input/1.in
	./programaTrab4 < ./_input/4.in
test5:
	./programaTrab4 < ./_input/1.in
	./programaTrab4 < ./_input/4.in
	./programaTrab4 < ./_input/5.in
test6:
	./programaTrab4 < ./_input/1.in
	./programaTrab4 < ./_input/6.in
test7:
	./programaTrab4 < ./_input/1.in
	./programaTrab4 < ./_input/7.in
test8:
	./programaTrab4 < ./_input/8.in
test9:
	./programaTrab4 < ./_input/9.in
test10:
	./programaTrab4 < ./_input/1.in
	./programaTrab4 < ./_input/10.in
test11:
	./programaTrab4 < ./_input/1.in
	./programaTrab4 < ./_input/10.in
	./programaTrab4 < ./_input/11.in
testCustom:
	./programaTrab4 < ./_input/custom.in
dump:
	hexdump -Cv arquivoTrab1.bin > ./_output/out.txt
clean:
	rm *.o
	rm programaTrab4
	rm *.txt