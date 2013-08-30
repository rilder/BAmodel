all: bamodel

# Useful variables
CFLAGS = -Wall -pedantic -O3
CC = gcc

# Main program
bamodel: main.c mtwister.h mtwister.c
	$(CC) $(CFLAGS) main.c -o bamodel -lm

clean:
	rm bamodel

	
