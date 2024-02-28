CXX = gcc

CFLAGS = -g -Wall

TARGET = countdown

.PHONY: all countdown clean

all: $(TARGET)

countdown: $(TARGET)

$(TARGET): countdown.c
	$(CXX) $(CFLAGS) -o $(TARGET) countdown.c

clean:
	rm -f $(TARGET)