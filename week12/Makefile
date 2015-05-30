# Compiles and tests the solutions to week 8 lab exercises.

CC=gcc
CFLAGS=-Wall -ansi 

all:		adjacent-sum poker rocket slides.pdf

adjacent-sum:	adjacent-sum.o
	$(CC) $(CFLAGS) -o adjacent-sum adjacent-sum.o

poker:		poker.o
	$(CC) $(CFLAGS) -o poker poker.o

rocket:		rocket.o
	$(CC) $(CFLAGS) -o rocket rocket.o

# target to build slides from LaTeX code.
slides.out:		slides.tex
	pdflatex slides.tex

slides.pdf:		slides.out
	pdflatex slides.tex

clean:
	rm adjacent-sum.o poker.o rocket.o
	rm slides.aux slides.log slides.nav slides.out slides.snm \
	    slides.toc

scrub:		clean
	rm adjacent-sum poker rocket slides.pdf


.PHONY:		all clean scrub

# vim: ts=8 sw=4 noet
