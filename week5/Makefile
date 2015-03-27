# Compiles and tests the solutions to week 5 lab exercises.
#
# To run this, type "make me a sandwich" in your terminal.

CC=gcc
CFLAGS=-Wall -ansi

perfect:	perfect.o sumfactors.o
	$(CC) $(CFLAGS) -o perfect perfect.o sumfactors.o

amicable:	amicable.o sumfactors.o
	$(CC) $(CFLAGS) -o amicable amicable.o sumfactors.o

perfect.o:	perfect.c sumfactors.h
sumfactors.o:	sumfactors.c sumfactors.h
amicable.o:	amicable.c sumfactors.h

me a:

sandwich:	perfect 


.PHONY:		me a sandwich

# vim: ts=8 sw=4 noet
