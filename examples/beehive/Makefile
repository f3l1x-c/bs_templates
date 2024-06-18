CFLAGS = -std=c11 -Wall -Wextra -D_DEFAULT_SOURCE

.PHONY: all clean

all: clean beehive

clean:
	rm -f *.o beehive

beehive: beehive.o
	$(CC) $^ -o $@

beehive.o: beehive.c
	@echo "Compiling $(basename $@)"
	$(CC) $(CFLAGS) -c $< -o $@