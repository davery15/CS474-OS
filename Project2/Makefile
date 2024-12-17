# Project2 CS 474
# Davery Valdez
# October 2024
#
# makefile for project2

CC = gcc
CFLAGS = -pthread -Wall -Wextra -O2

# Target executable
TARGET = project2

# Source files
SRCS = project2.c thread_proc.c

# Object files
OBJS = $(SRCS:.c=.o)

# Build target
all: $(TARGET)

# Linking step
$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS)

# Compilation step
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean target to remove compiled files
clean:
	rm -f $(OBJS) $(TARGET)

# Run target with default 4 threads
run: $(TARGET)
	./$(TARGET) 4

# Run with a custom number of threads
run_t:
	./$(TARGET) $(THREADS)