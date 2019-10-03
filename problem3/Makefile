CFLAGS=-std=gnu99 -Wall -Werror -pedantic
CC=gcc

.PHONY: all clean clear

perf: perf.c
	$(CC) $(CFLAGS) perf.c -o perf

clean:
	rm -f *~ perf

clear:
	rm -f *~

debug: perf.c
	$(CC) $(CFLAGS) -ggdb3 perf.c -o perf

o1: perf.c
	$(CC) $(CFLAGS) -O1 perf.c -o perf

o2: perf.c
	$(CC) $(CFLAGS) -O2 perf.c -o perf

o3: perf.c
	$(CC) $(CFLAGS) -O3 perf.c -o perf
