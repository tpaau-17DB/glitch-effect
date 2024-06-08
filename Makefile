CC = g++

SRC = src
BIN = bin
CFLAGS = -Wall -g -std=c++17 -lncurses

TARGET = glitch

SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

all: pre-build $(TARGET)

pre-build:
	mkdir -p $(BIN)

$(TARGET): $(OBJS)
	$(CC) -o $(BIN)/$(TARGET) $(OBJS) $(CFLAGS)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/*.o $(BIN)/$(TARGET)

.PHONY: all clean
