CC = g++

SRC = src
BIN = bin

CFLAGS = -Wall -g -std=c++17
NCURSES_FLAG = -lncurses

TARGET = glitch

SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

# List all ncurses related source files here
NCURSES_SRCS = $(SRC)/glitch.cpp \
               $(SRC)/logger.cpp

NCURSES_OBJS = $(NCURSES_SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

# All object files except those that need ncurses
OTHER_OBJS = $(filter-out $(NCURSES_OBJS), $(OBJS))

all: pre-build $(TARGET)

pre-build:
	mkdir -p $(BIN)

$(TARGET): $(OBJS)
	$(CC) -o $(BIN)/$(TARGET) $(OTHER_OBJS) $(NCURSES_OBJS) $(CFLAGS) $(NCURSES_FLAG)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/*.o $(BIN)/$(TARGET)

.PHONY: all clean
