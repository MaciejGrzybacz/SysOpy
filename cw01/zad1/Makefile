##Static library
#CC=gcc
#CFLAGS=-g -Wall
#OBJECTS=main.o collatz.o
#EXECUTABLE=prog
#TARGET=prog
#LIBRARY=libcollatz.a
#
#$(TARGET) : $(OBJECTS) $(LIBRARY)
#	$(CC) $(CFLAGS) $(OBJECTS) -L. -lcollatz -o $(TARGET)
#
#$(LIBRARY): collatz.o
#	ar rcs $(LIBRARY) collatz.o
#
#main.o: main.c collatz.h
#	$(CC) $(CFLAGS) -c main.c
#
#collatz.o: collatz.h collatz.c
#	$(CC) $(CFLAGS) -c collatz.c
#
#.PHONY : clean
#
#clean:
#	@rm -f $(TARGET) $(OBJECTS) $(LIBRARY) core

#Shared library
#CC=gcc
#CFLAGS=-g -Wall
#OBJECTS=main.o collatz.o
#EXECUTABLE=prog
#TARGET=prog
#LIBRARY=libcollatz.so
#
#$(TARGET) : $(OBJECTS) $(LIBRARY)
#	$(CC) $(CFLAGS) $(OBJECTS) -L. -lcollatz -o $(TARGET)
#
#$(LIBRARY): collatz.o
#	$(CC) -shared -o $(LIBRARY) collatz.o
#
#main.o: main.c collatz.h
#	$(CC) $(CFLAGS) -c main.c
#
#collatz.o: collatz.h collatz.c
#	$(CC) $(CFLAGS) -fPIC -c collatz.c
#
#.PHONY : clean
#
#clean:
#	@rm -f $(TARGET) $(OBJECTS) $(LIBRARY) test.o core
#

# Both static and dynamic linking
CC=gcc
CFLAGS=-g -Wall
OBJECTS=main.o collatz.o
EXECUTABLE=prog
TARGET=prog
STATIC_LIBRARY=libcollatz.a
SHARED_LIBRARY=libcollatz.so

$(TARGET) : $(OBJECTS) $(STATIC_LIBRARY) $(SHARED_LIBRARY)
	$(CC) $(CFLAGS) $(OBJECTS) -o $(TARGET) -L. -lcollatz

$(STATIC_LIBRARY): collatz.o
	ar rcs $(STATIC_LIBRARY) collatz.o

$(SHARED_LIBRARY): collatz.o
	$(CC) -shared -o $(SHARED_LIBRARY) collatz.o

main.o: main.c collatz.h
	$(CC) $(CFLAGS) -c main.c

collatz.o: collatz.h collatz.c
	$(CC) $(CFLAGS) -fPIC -c collatz.c

.PHONY : clean

clean:
	rm -f $(TARGET) $(OBJECTS) $(STATIC_LIBRARY) $(SHARED_LIBRARY) core
