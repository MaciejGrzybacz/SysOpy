
CC = gcc
CFLAGS = -Wall -O0

all: main

main: z1.c
	$(CC) $(CFLAGS) -o main z1.c

clean:
	rm -f main *.o

test:
	./main 0.0000000001 10


