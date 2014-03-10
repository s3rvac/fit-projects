# makefile pro preklad LaTeX verze PDS projektu
# Autor: Petr Pospichal (xpospi45)
# Upravil: Jaroslav Dytrych (xdytry00), Petr Zemek (xzemek02)

ARCHIV=manual.pdf
CO=manual
LITERATURA=literatura

all: $(CO).pdf

pdf: $(CO).pdf


$(CO).pdf: clean
	pdflatex $(CO)
	bibtex $(CO)
	pdflatex $(CO)
	pdflatex $(CO)

clean:
	rm -f *.dvi *.log $(CO).blg $(CO).bbl $(CO).toc *.aux $(CO).out

pack:
	tar czvf $(ARCHIV).tar.gz *.tex *.bib *.pdf *.jpg *.bst Makefile
