CC = gcc
CFLAGS = -Wall -O0
LDFLAGS= -Lcollatz_library collatz
LIB_DIR = collatz_library

# Compile all targets: static_agent, shared_agent, dl_agent
all : static_agent shared_agent dl_agent

# Build static_agent executable
static_agent : static_agent.c
	# Enter collatz_library directory and make (compile) its contents
	make -C collatz_library
    # Compile static_agent.c and link with static library collatz.a
	$(CC) static_agent.c -o static_agent -L$(LIB_DIR) -l:collatz.a -lm

# Build shared_agent executable
shared_agent : shared_agent.c
	# Enter collatz_library directory and make (compile) its contents
	make -C collatz_library
    # Compile shared_agent.c and link with shared library collatz.so
	$(CC) $(CDFLAGS) -L ./collatz_library shared_agent.c ./collatz_library/collatz.so -Wl,-rpath=./collatz_library -o shared_agent -lm

# Build dl_agent executable (for loading dynamic library at runtime)
dl_agent : dl_agent.c
    # Enter collatz_library directory and make (compile) its contents
	make -C collatz_library
    # Compile dl_agent.c with USE_DYNAMIC_LIBRARY flag to indicate dynamic library usage, and link with dl library
	$(CC) $(CFLAGS) -DUSE_DYNAMIC_LIBRARY -o dl_agent dl_agent.c -ldl

# Clean up generated files and directories
clean :
    # Remove executables and object files
	rm -f static_agent shared_agent dl_agent *.o *.asm
    # Clean collatz_library directory
	make -C collatz_library clean
