CC := gcc
CFLAGS := -c
SOURCE_DIR := src
OUT_DIR := src

.PHONY: all clean

all: src/Events.o

%.o: %.c .FORCE
	$(CC) $(CFLAGS) $< -o $@

clean:
	@rm -rf src/*.o
