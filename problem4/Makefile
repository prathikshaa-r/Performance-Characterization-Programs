CFLAGS=-std=gnu99 -Wall -Werror -pedantic
CC=gcc

.PHONY: all clean clear

mmul: mmul.c
	$(CC) $(CFLAGS) mmul.c -o mmul

clean:
	rm -f *~ mmul

clear:
	rm -f *~

debug: mmul.c
	$(CC) $(CFLAGS) -ggdb3 mmul.c -o mmul

o1: mmul.c
	$(CC) $(CFLAGS) -O1 mmul.c -o mmul

o2: mmul.c
	$(CC) $(CFLAGS) -O2 mmul.c -o mmul

o3: mmul.c
	$(CC) $(CFLAGS) -O3 mmul.c -o mmul
