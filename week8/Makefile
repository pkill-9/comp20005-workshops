# Compiles and tests the solutions to week 8 lab exercises.

CC=gcc
CFLAGS=-Wall -ansi

all:		freq popular

freq:		histogram.o
	$(CC) $(CFLAGS) -o freq histogram.o

popular:	popular.o
	$(CC) $(CFLAGS) -o popular popular.o

.PHONY:		all

# vim: ts=8 sw=4 noet
