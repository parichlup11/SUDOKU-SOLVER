CC = gcc
CFLAGS = -std=c99 -Wall -Wextra -pedantic -O2

TARGET = sudoku

SOURCES = main.c sudoku.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES)

clean:
	rm -f $(TARGET) *.o

.PHONY: all clean