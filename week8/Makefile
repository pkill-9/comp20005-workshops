# Compiles and tests the solutions to week 8 lab exercises.

CC=gcc
CFLAGS=-Wall -ansi

all:		freq popular anagram ssort listchange

freq:		histogram.o
	$(CC) $(CFLAGS) -o freq histogram.o

popular:	popular.o
	$(CC) $(CFLAGS) -o popular popular.o

anagram:	anagram.o
	$(CC) $(CFLAGS) -o anagram anagram.o

ssort:		selection_sort.o
	$(CC) $(CFLAGS) -o ssort selection_sort.o

listchange:	listchange.o
	$(CC) $(CFLAGS) -o listchange listchange.o

.PHONY:		all

# vim: ts=8 sw=4 noet
