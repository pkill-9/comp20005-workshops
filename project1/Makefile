# Real simple makefile for the project. The purpose of this is really just
# to avoid having to type gcc blah blah blah all the time; we don't have to
# compile multiple files or anything.

CC = gcc
CFLAGS = -Wall -ansi

polygons:		polygons.o
	$(CC) $(CFLAGS) -o polygons polygons.o -lm

# vim: ts=8 sw=4 noet
