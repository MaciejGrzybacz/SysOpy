CC = gcc
CFLAGS = -Wall -O0

all: client server

client: client.c
	$(CC) $(CFLAGS) -o client client.c

server : server.c
	rm -rf /dev/mqueue/*
	$(CC) $(CFLAGS) -o server server.c

clean:
	rm -f server client rm -rf /dev/mqueue/* *.o


