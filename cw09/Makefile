CC = gcc
BUILD = debug

CFLAGS_COMMON = -std=c11 -Wall

ifeq ($(BUILD), debug)
    CFLAGS = -g -O0 $(CFLAGS_COMMON)
else
    CFLAGS = -O2 $(CFLAGS_COMMON)
endif

.PHONY: all clean

all: life

life: grid.c life.c
	$(CC) $(CFLAGS) grid.c life.c -o life -lncursesw -lpthread -lm

run: life
	./life 12

clean:
	rm -f life
