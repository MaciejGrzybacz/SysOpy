CC = gcc
CFLAGS = -Wall

# Default target to build both static and shared libraries
all :
	make static
	make shared

# Target to build the static library
static : collatz.c
    # Compile collatz.c into an object file
	$(CC) -c collatz.c
    # Create the static library collatz.a by archiving collatz.o
	ar rcs collatz.a collatz.o

# Target to build the shared library
shared: collatz.c
    # Compile collatz.c into a position-independent code (PIC) object file
	$(CC) -fPIC -c collatz.c
    # Create the shared library collatz.so from collatz.o
	$(CC) -shared collatz.o -o collatz.so

# Target to clean up generated files
clean :
    # Remove the static library, shared library, and object file
	rm -f collatz.a collatz.so collatz.o
