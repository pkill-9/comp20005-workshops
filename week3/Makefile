# Compiles and tests the solutions to week 3 lab exercises.
#
# To run this, type "make it so" in your terminal.

listchange:	listchange.c
	gcc -Wall -ansi -o listchange listchange.c

convert:	convert.c
	gcc -Wall -ansi -o convert convert.c

it:		listchange convert

so:
	./testchange.sh

.PHONY:		it so

# vim: ts=8 sw=4 noet
