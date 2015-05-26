
all:			slides.pdf

# target to build slides from LaTeX code.
slides.out:		slides.tex
	pdflatex slides.tex

slides.pdf:		slides.out
	pdflatex slides.tex

clean:
	rm slides.aux slides.log slides.nav slides.out slides.snm \
	    slides.toc

scrub:			clean
	rm slides.pdf

.PHONY:			all clean scrub


# vim: ts=8 sw=4 noet
