# Compiles and tests the solutions to week 8 lab exercises.

CC=gcc
CFLAGS=-Wall -ansi -O0 -g -pg

all:		adjacent-sum

adjacent-sum:	adjacent-sum.o
	$(CC) $(CFLAGS) -o adjacent-sum adjacent-sum.o

clean:
	rm adjacent-sum.o

scrub:		clean
	rm adjacent-sum


.PHONY:		all clean scrub

# vim: ts=8 sw=4 noet
