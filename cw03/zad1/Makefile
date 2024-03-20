CC = gcc
CFLAGS = -Wall -O0
SRC = z1.c
OBJ_BYTE = z1_byte.o
OBJ_BLOCK = z1_block.o

all: byte block

byte: $(OBJ_BYTE)
	$(CC) $(CFLAGS) $(OBJ_BYTE) -o byte

block: $(OBJ_BLOCK)
	$(CC) $(CFLAGS) $(OBJ_BLOCK) -o block

z1_byte.o: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ_BYTE) -D BYTE_MODE

z1_block.o: $(SRC)
	$(CC) $(CFLAGS) -c $(SRC) -o $(OBJ_BLOCK)

clean:
	rm -f *.o byte block *.txt

byte_test:
	./byte ../large_file.txt byte_out1.txt
	./byte byte_out1.txt byte_out2.txt
	diff -s ../large_file.txt byte_out2.txt

block_test:
	./block ../large_file.txt block_out1.txt
	./block block_out1.txt block_out2.txt
	diff -s ../large_file.txt block_out2.txt

make test:
	make byte_test
	make block_test