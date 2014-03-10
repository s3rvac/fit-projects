#
# Encoding: latin2
# Project:  ITY - Project 3
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - compile to pdf:        make
#   - clean:                 make clean
#   - pack (zip):            make pack
#

PROJ=proj3

all: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex
	cslatex $(PROJ).tex
	cslatex $(PROJ).tex
	dvips -T 210mm,295mm $(PROJ).dvi
	ps2pdf $(PROJ).ps

clean:
	rm -f *.dvi *.ps *.log $(PROJ).blg $(PROJ).bbl $(PROJ).toc *.aux $(PROJ).out

pack:
	zip xzemek02-fit.zip Makefile *.tex fitprj.cls
