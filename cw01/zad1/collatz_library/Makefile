CC = gcc
CFLAGS = -Wall

.PHONY : all static shared

all :
	make static
	make shared

static : collatz.c
	$(CC) -c collatz.c
	ar rcs collatz.a collatz.o

shared: collatz.c
	$(CC) -fPIC -c collatz.c
	$(CC) -shared collatz.o -o  collatz.so

clean :
	rm -f collatz.a collatz.so collatz.o
