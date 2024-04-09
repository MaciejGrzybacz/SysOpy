CC = gcc
CFLAGS = -Wall -O0
LIB_PATH=lib
LIB_NAME=calc_lib
LIB_SO=lib/calc_lib.so

all: static_main shared_main dynamic_main

static_main: main.c
	make -C $(LIB_PATH)
	$(CC) $(CFLAGS) main.c -o static_main -Llib -l:calc_lib.a -lm

shared_main: main.c
	make -C $(LIB_PATH)
	$(CC) $(CFLAGS) main.c  -L $(LIB_PATH) $(LIB_SO) -Wl,-rpath=$(LIB_PATH) -o shared_main -lm

dynamic_main: main.c
	make -C $(LIB_PATH)
	$(CC) $(CFLAGS) main.c -o dynamic_main -ldl -DUSE_DYNAMIC_LIBRARY

test:
	./static_main
	./shared_main
	./dynamic_main
clean:
	rm -f *_main *.o
	make -C $(LIB_PATH) clean

