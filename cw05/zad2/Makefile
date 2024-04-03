CC = gcc

all: catcher sender

catcher: catcher.c
	$(CC) -Wall -O0 -o catcher catcher.c

sender: sender.c
	$(CC) -Wall -O0 -o sender sender.c

clean:
	rm -f catcher sender *.o

PID := 0
ARGUMENT := 1
run_sender: sender
	./sender $(PID) $(ARGUMENT)
run_catcher: catcher
	./catcher


