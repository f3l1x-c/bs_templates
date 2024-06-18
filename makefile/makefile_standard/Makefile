.PHONY: all clean

CC = gcc
CFLAGS += -std=c11 -Wall -Wextra -Werror
TARGETS = filename

all: $(TARGETS)

filename: filename.o
	$(CC) $^ -o $@

filename.o: filename.c
	@echo "Compiling $(basename $@):"
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	$(RM) $(TARGETS) *.o