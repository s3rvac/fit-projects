#
# Encoding: latin2
# Project:  ITY - Project 4
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile to pdf:        make
#   - clean:                 make clean
#   - pack (zip):            make pack
#

PROJ=proj4

all: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex $(PROJ).bib
	pdfcslatex $(PROJ)
	bibtex $(PROJ)
	pdfcslatex $(PROJ)
	pdfcslatex $(PROJ)

clean:
	rm -f *.dvi *.ps *.log $(PROJ).blg $(PROJ).bbl $(PROJ).toc *.aux $(PROJ).out

pack:
	zip xzemek02-fit.zip Makefile $(PROJ).tex $(PROJ).bib czechiso.bst
