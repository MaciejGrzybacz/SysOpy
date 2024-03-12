CC = gcc
CFLAGS = -Wall -O0

all: main

main: z1.c
	$(CC) $(CFLAGS) -o main z1.c

clean:
	rm -f main blockfile.txt bytefile.txt

test:
	./main