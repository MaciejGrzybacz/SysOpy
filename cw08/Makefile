CC = gcc
CFLAGS_COMMON := -std=c11
CFLAGS := $(CFLAGS_COMMON) -O0

all: printers users

printers_system: printers.c
	$(CC) $(CFLAGS) -o printers printers.o -lrt -lpthread

users_simulator: users.c
	rm -rf /dev/mqueue/*
	$(CC) $(CFLAGS) -o users users.o -lrt -lpthread

clean:
	rm -rf /dev/mqueue/* *.o printers users

