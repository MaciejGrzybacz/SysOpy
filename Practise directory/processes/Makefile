CC = gcc
CFLAGS = -Wall -O0

all: main

main: main.c
	$(CC) $(CFLAGS) main.c -o main

test:
	./main 10

clean:
	rm -f main *.o
