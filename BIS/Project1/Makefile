#
# Encoding: utf-8
# Project:	BIS, 1st project
# Author:	Petr Zemek, xzemek02@stud.fit.vutbr.cz, 2009
#
# GNU Makefile for project documentation.
#

PROJ = xzemek02-proj1

all: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex
	pdfcslatex $(PROJ)
	pdfcslatex $(PROJ)

clean:
	rm -f *.dvi *.log $(PROJ).blg $(PROJ).bbl $(PROJ).toc *.aux $(PROJ).out
