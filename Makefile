all: bamodel

# Useful variables
INCLUDE_DIR = ./include/
SRC_DIR = ./src/
LIB_DIR = ./lib/
CFLAGS = -Wall -pedantic -O3
CC = gcc

# Main program
bamodel: $(LIB_DIR)main.o $(LIB_DIR)mtwister.o
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(LIB_DIR)main.o $(LIB_DIR)mtwister.o -o bamodel -lm

$(LIB_DIR)main.o: $(SRC_DIR)main.c $(INCLUDE_DIR)mtwister.h
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC_DIR)main.c -c -o $(LIB_DIR)main.o

$(LIB_DIR)mtwister.o: $(INCLUDE_DIR)mtwister.h $(SRC_DIR)mtwister.c
	$(CC) $(CFLAGS) -I$(INCLUDE_DIR) $(SRC_DIR)mtwister.c -c -o $(LIB_DIR)mtwister.o

clean:
	rm bamodel

	
