#-*- Makefile -*-

all: minesweeper.o helpers.o
	gcc minesweeper.o helpers.o -o minesweeper

minesweeper.o: minesweeper.c helpers.h
	gcc minesweeper.c -c -o minesweeper.o

helpers.o: helpers.c helpers.h
	gcc helpers.c -c -o helpers.o

clean:
	rm *.o minesweeper