CC = gcc
CFLAGS = -Wall -O0
LDFLAGS= -Lcollatz_library collatz
LIB_DIR = collatz_library

all : static_client shared_client dynamic_client

# Build static_client executable
static_client : client.c
	# Enter collatz_library directory and make its contents
	make -C collatz_library
    # Compile client.c and link with static library collatz.a
	$(CC) client.c -o static_client -L$(LIB_DIR) -l:collatz.a -lm

# Build shared_client executable
shared_client : client.c
	# Enter collatz_library directory and make its contents
	make -C collatz_library
    # Compile shared_client.c and link with shared library collatz.so
	$(CC) $(CDFLAGS) client.c  -L ./collatz_library ./collatz_library/collatz.so -Wl,-rpath=./collatz_library -o shared_client -lm

# Build dynamic_client executable (for loading dynamic library at runtime)
dynamic_client : client.c
    # Enter collatz_library directory and make its contents
	make -C collatz_library
    # Compile dynamic_client.c with USE_DYNAMIC_LIBRARY flag to indicate dynamic library usage, and link with dl library
	$(CC) $(CFLAGS) client.c -o dynamic_client -ldl -DUSE_DYNAMIC_LIBRARY

# Run all executables
test:
	./static_client
	./shared_client
	./dynamic_client

# Generate object dump of all executables
objdump:
	objdump -d static_client > static_client.objdump
	objdump -d shared_client > shared_client.objdump
	objdump -d dynamic_client > dynamic_client.objdump
	cat static_client.objdump
	cat shared_client.objdump
	cat dynamic_client.objdump

# Clean up generated libs and directories
clean :
    # Remove executables and object libs
	rm -f static_client shared_client dynamic_client *.o *.objdump
    # Clean collatz_library directory
	make -C collatz_library clean