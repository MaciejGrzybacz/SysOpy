CC = gcc
CFLAGS = -Wall -O0

all: sender receiver

sender: sender.c
	$(CC) $(CFLAGS) sender.c -o sender

receiver: receiver.c
	$(CC) $(CFLAGS) receiver.c -o receiver

clean:
	rm -f fifo receiver sender *.o