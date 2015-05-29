# Compiles and tests the solutions to week 8 lab exercises.

CC=gcc
CFLAGS=-Wall -ansi 

all:		adjacent-sum poker

adjacent-sum:	adjacent-sum.o
	$(CC) $(CFLAGS) -o adjacent-sum adjacent-sum.o

poker:		poker.o
	$(CC) $(CFLAGS) -o poker poker.o

clean:
	rm adjacent-sum.o poker.o

scrub:		clean
	rm adjacent-sum poker


.PHONY:		all clean scrub

# vim: ts=8 sw=4 noet
