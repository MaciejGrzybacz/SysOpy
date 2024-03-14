CC = gcc
CFLAGS = -Wall -O0

all: main test

main: z1.c
	$(CC) $(CFLAGS) -o main z1.c

clean:
	rm -f main *.txt

test:
	./main
	cat pomiar_zad_2.txt