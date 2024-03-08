CC = gcc
CFLAGS = -Wall
LDFLAGS= -Lcollatz_library collatz
LIB_DIR = collatz_library

all : static_agent shared_agent dl_agent

static_agent : static_agent.c
	make -C collatz_library
	$(CC) static_agent.c -o static_agent -L$(LIB_DIR) -l:collatz.a -lm

shared_agent : shared_agent.c
	make -C collatz_library
	$(CC) $(CDFLAGS) -L ./collatz_library shared_agent.c ./collatz_library/collatz.so -Wl,-rpath=./collatz_library -o shared_agent -lm

dl_agent : dl_agent.c
	make -C collatz_library
	$(CC) $(CFLAGS) -o dl_agent dl_agent.c -ldl

clean :
	rm -f static_agent shared_agent dl_agent *.o
	make -C collatz_library clean
