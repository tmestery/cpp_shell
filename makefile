CC = g++ 
CFLAGS = -Wall -g -std=c++17
SRCS = src/main.cpp src/parse.cpp
TARGET = src/main

all:
	$(CC) $(CFLAGS) -o $(TARGET) $(SRCS)

clean:
	rm -f $(TARGET)