# Makefile for Recursion Test

CC        = g++
CXXFLAGS  = -std=c++11 -O3
TARGET    = RecursionTest

all: clean $(TARGET)

$(TARGET):
	$(CC) $(CXXFLAGS) $(TARGET).cpp -o $(TARGET)

run:
	./$(TARGET)

clean:
	$(RM) $(TARGET)

.PHONY: all clean

