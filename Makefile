CC = g++

SRC = src
BIN = bin

CFLAGS = -Wall -g -std=c++17
NCURSES_FLAG = -lncurses

TARGET = glitch
TARGET_DIR = /usr/bin/

SRCS = $(wildcard $(SRC)/*.cpp)
OBJS = $(SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

# List all ncurses related source files here
NCURSES_SRCS = $(SRC)/glitch.cpp \
               $(SRC)/logger.cpp

NCURSES_OBJS = $(NCURSES_SRCS:$(SRC)/%.cpp=$(BIN)/%.o)

# All object files except those that need ncurses
OTHER_OBJS = $(filter-out $(NCURSES_OBJS), $(OBJS))

MAKEFLAGS += -j

all: pre-build $(BIN)/$(TARGET)

pre-build:
	mkdir -p $(BIN)

$(BIN)/$(TARGET): $(OBJS)
	$(CC) -o $@ $(OTHER_OBJS) $(NCURSES_OBJS) $(CFLAGS) $(NCURSES_FLAG)

$(BIN)/%.o: $(SRC)/%.cpp
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(BIN)/*.o $(BIN)/$(TARGET)

install: $(BIN)/$(TARGET)
	@echo "Installing $(TARGET) to $(TARGET_DIR)"
	sudo cp $(BIN)/$(TARGET) $(TARGET_DIR)

uninstall:
	@echo "Removing $(TARGET) from $(TARGET_DIR)"
	sudo rm -f $(TARGET_DIR)/$(TARGET)

.PHONY: all clean install uninstall pre-build build

