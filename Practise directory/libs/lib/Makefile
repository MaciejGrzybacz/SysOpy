CC = gcc
CFLAGS = -Wall -O0

all: static_library shared_library

static_library: calc_lib.c
	$(CC) -c calc_lib.c
	ar rcs calc_lib.a calc_lib.o

shared_library:
	$(CC) -fPIC -c calc_lib.c
	$(CC) -shared calc_lib.o -o calc_lib.so

clean:
	rm -f *.a *.o *.so





