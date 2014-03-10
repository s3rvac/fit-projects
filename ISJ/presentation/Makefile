#
# Encoding: utf8
# Project:  ISJ - project presentation
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#

PROJ=presentation

all: $(PROJ).pdf

$(PROJ).pdf: $(PROJ).tex
	pdfcslatex $(PROJ)
	pdfcslatex $(PROJ)

clean:
	rm -f *.dvi *.log $(PROJ).blg $(PROJ).bbl $(PROJ).toc *.aux $(PROJ).out
