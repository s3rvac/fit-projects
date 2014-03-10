#
# Encoding: utf8
# Project:  ITY - Project 2
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile to pdf:        make
#   - clean:                 make clean
#   - pack (zip):            make pack
#

PROJ=proj2

all: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex
	pdfcslatex $(PROJ).tex
	pdfcslatex $(PROJ).tex

clean:
	rm -f *.dvi *.ps *.log $(PROJ).blg $(PROJ).bbl $(PROJ).toc *.aux $(PROJ).out

pack:
	zip xzemek02-fit.zip Makefile *.tex
