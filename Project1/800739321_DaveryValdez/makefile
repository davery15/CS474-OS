# Project1 CS 474
# Davery Valdez
# September 2024
#
# makefile for project1

CC = gcc
CFLAGS = -Wall -g
all: project1 child_proc
project1: project1.o
	$(CC) $(CFLAGS) -o project1 project1.o
child_proc: child_proc.o
	$(CC) $(CFLAGS) -o child_proc child_proc.o
%.o: %.c project1.h
	$(CC) $(CFLAGS) -c $<
clean:
	rm -f *.o project1 child_proc