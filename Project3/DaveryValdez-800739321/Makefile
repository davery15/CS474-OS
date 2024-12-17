# Project3 CS 474
# Davery Valdez
# October 2024
#
# makefile for project3

CC = gcc
CFLAGS = -lpthread
TARGET = scheduling_program
SRCS = main.c scheduling_algorithms.c

all: $(TARGET)

$(TARGET): $(SRCS)
	$(CC) -o $(TARGET) $(SRCS) $(CFLAGS)

clean:
	rm -f $(TARGET)