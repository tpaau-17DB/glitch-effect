CC = g++

SRC = src
BIN = bin
CFLAGS = -Wall -g -std=c++17 -lncurses

all: pre-build glitch

pre-build:
	mkdir -p $(BIN)

glitch: $(SRC)/glitch.cpp
	$(CC) -o $(BIN)/glitch $(SRC)/glitch.cpp $(CFLAGS)

clean:
	rm -f glitch

.PHONY: all clean
