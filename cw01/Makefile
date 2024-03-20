CC=gcc
CFLAGS=-c -Wall

all: countdown

hello: countdown.o second.o
	$(CC) countdown.o second.o -o countdown

countdown.o: countdown.c
	$(CC) $(CFLAGS) countdown.c

second.o: second.c
	$(CC) $(CFLAGS) second.c

clean:
	rm -f *.o countdown
