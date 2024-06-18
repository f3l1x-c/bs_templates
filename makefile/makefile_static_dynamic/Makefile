CC = cc
CFLAGS1 = -std=c11 -Wall -Wextra -pedantic -fpic
CFLAGS2 = -std=c11 -Wall -Wextra -pedantic

all: clean some_library.so my_program

# dynamic linking
some_library.so: some_library.o
	$(CC) -shared -o some_library.so some_library.o

some_library.o: some_library.c
	$(CC) $(CFLAGS1) -c some_library.c -o some_library.o

# static linking
# if not working use -static-libgcc instead of -static
my_program: my_program.o
	$(CC) -static -o my_program my_program.o

my_program.o: my_program.c
	$(CC) $(CFLAGS2) -c my_program.c -o my_program.o

clean:
	rm -f my_program my_program.o some_library.o