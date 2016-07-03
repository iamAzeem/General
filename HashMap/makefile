# Makefile for HashMap Test

CC        = g++
CXXFLAGS  = -std=c++14 -O3 -Wall -Werror
SOURCES   = hashmap.cpp HashMapTest.cpp
TARGET    = HashMapTest

all: $(TARGET)

$(TARGET):
	$(CC) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

run:
	./$(TARGET)

clean:
	$(RM) $(TARGET)

.PHONY: all clean

