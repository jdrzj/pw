all: init producent konsument
init: init.c
	gcc -o init init.c -lrt
producent: producent.c
	gcc -o producent producent.c -lrt
konsument: konsument.c
	gcc -o konsument konsument.c -lrt
clean:
	rm init producent konsument
