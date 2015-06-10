# Real simple makefile for the project. The purpose of this is really just
# to avoid having to type gcc blah blah blah all the time; we don't have to
# compile multiple files or anything.

CC = gcc
CFLAGS = -Wall 

csv-analyse:		csv-analyse.o
	$(CC) $(CFLAGS) -o csv-analyse csv-analyse.o -lm

random_input:		random_input.o
	$(CC) $(CFLAGS) -o random_input random_input.o

tags:
	cscope -b

.PHONY:			tags

# vim: ts=8 sw=4 noet
