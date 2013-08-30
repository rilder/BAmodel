all: bamodel

# Useful variables
INCLUDE_DIR = ./include/
SRC_DIR = ./src/
LIB_DIR = ./lib/
CFLAGS = -Wall -pedantic -O3
CC = gcc

# Main program
bamodel: main.c mtwister.h mtwister.c
	$(CC) $(CFLAGS) main.c -o bamodel -lm

clean:
	rm bamodel

	
