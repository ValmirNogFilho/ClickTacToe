all: main

main: tabuleiro.o main.c constantes.h
	rm -f main
	gcc -o main tabuleiro.o main.c -lintelfpgaup -std=c99
	rm -f tabuleiro.o
	clear

tabuleiro.o: tabuleiro.c tabuleiro.h
	gcc -c tabuleiro.c -std=c99

run: main
	sudo ./main
