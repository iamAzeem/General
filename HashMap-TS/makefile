# Makefile for HashMap Test

CC        = g++
CXXFLAGS  = -std=c++14 -O3 -g3 -Wall
LDFLAGS   = -pthread
SOURCES   = read_write_lock.cpp HashMapTest.cpp
TARGET    = HashMapTest

all: clean $(TARGET)

$(TARGET):
	$(CC) $(CXXFLAGS) $(SOURCES) -o $(TARGET) $(LDFLAGS)

run:
	./$(TARGET)

run-v:
	valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./$(TARGET)

clean:
	$(RM) $(TARGET)

.PHONY: all clean run run-v

