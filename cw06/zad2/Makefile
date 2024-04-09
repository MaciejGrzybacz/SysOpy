CC = gcc
CFLAGS = -Wall -O0

all: receiver sender

receiver: receiver.c
	$(CC) $(CFLAGS) -o receiver receiver.c

sender : sender.c
	$(CC) $(CFLAGS) -o sender sender.c


clean:
	rm -f sender receiver *fifo *.o

test:
	./sender

