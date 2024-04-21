mCC = gcc
CFLAGS = -Wall -O0

all: main

main: z1.c
	$(CC) $(CFLAGS) -o main z1.c

clean:
	rm -f main *.o

test:
	./main ignore
	./main handler
	./main mask
	./main asraer
	./main none


