CC = g++
SRC = src
BIN = bin

CFLAGS = -Wall -g -std=c++17
NCURSES_FLAG = -lncurses

TARGET = glitch

SRCS = $(wildcard $(SRC)/*.cpp)

# List all ncurses-related source files here
NCURSES_SRCS = $(SRC)/glitch.cpp \
               $(SRC)/logger.cpp

# Standard sources that do not use ncurses
STANDARD_SRCS = $(SRC)/arginterpreter.cpp \
                $(SRC)/printer.cpp \
		$(SRC)/asciiBuffer.cpp \
		$(SRC)/fileLoader.cpp \
		$(SRC)/configLoader.cpp

# Objects that need ncurses to compile
NCURSES_OBJS = $(NCURSES_SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

# Objects that do not need ncurses
OTHER_OBJS = $(STANDARD_SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

# All object files
OBJS = $(NCURSES_OBJS) $(OTHER_OBJS)

all: pre-build $(TARGET)

pre-build:
	mkdir -p $(BIN)

$(TARGET): $(OBJS)
	$(CC) -o $(BIN)/$(TARGET) $(OBJS) $(CFLAGS) $(NCURSES_FLAG)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/*.o $(BIN)/$(TARGET)

.PHONY: all clean
