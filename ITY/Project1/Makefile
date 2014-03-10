#
# Encoding: utf8
# Project:  ITY - Project 1
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile to pdf:        make
#   - clean:                 make clean
#   - pack (zip):            make pack
#

PROJ=proj1

all: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex
	latex $(PROJ).tex
	dvips $(PROJ).dvi
	ps2pdf $(PROJ).ps

clean:
	rm -f *.dvi *.ps *.log $(PROJ).blg $(PROJ).bbl $(PROJ).toc *.aux $(PROJ).out

pack:
	zip xzemek02-fit.zip Makefile *.tex
