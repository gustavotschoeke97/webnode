SOURCE_FILES = ddll/DDLL.c weblist.c main.c
OUTPUT_FILE = main.out

CC = gcc
CFLAGS = -Wall -std=c99 -g

all:
	$(CC) $(CFLAGS) $(SOURCE_FILES) -o $(OUTPUT_FILE) -lm

run: all
	./$(OUTPUT_FILE)

clean:
	rm -f $(OUTPUT_FILE)
