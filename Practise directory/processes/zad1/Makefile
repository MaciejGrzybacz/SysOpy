CC=gcc
CFLAGS=-Wall -O0

bibl.o:
	$(CC) $(CFLAGS) -c -fPIC bibl1.c -o bibl.o

bibl.so: bibl.o
	$(CC) -shared bibl.o -o bibl.so

dynamic: dynamic.o bibl.so
	$(CC) $(CFLAGS) dynamic.o -Wl,-rpath,. -o dynamic

dynamic.o:
	$(CC) $(CFLAGS) -c zad1.c -o dynamic.o

clean:
	rm -rf dynamic dynamic.o bibl.o bibl.so