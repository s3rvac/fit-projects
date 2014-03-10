#
# Makefile - GNU - cslatex
# Project: IFJ06
# Authors:
# 	Lukas Marsik, xmarsi03@stud.fit.vutbr.cz
#	Libor Polcak, xpolca03@stud.fit.vutbr.cz
#	Boris Prochazka, xproch63@stud.fit.vutbr.cz
#	Martin Tomec, xtomec05@stud.fit.vutbr.cz
#	Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#  - pdf document:			make
#  - ps document:				make ps
#	- color version:			make color
#	- compress (zip)			make pack
#  - clean (delete)			make clean
#
PROJECT = IFJ
SRC = parser.tex

LATEX   = cslatex
DVI2PDF = dvipdf
DVI2PS  = dvips

LATEX_FLAGS   =
DVI2PDF_FLAGS =
DVI2PS_FLAGS  = -o dokumentace.ps


all: dokumentace.pdf
	rm -f *.dvi *.log  *.aux

ps: dokumentace.ps
	rm -f *.dvi *.log  *.aux

verbose : dokumentace.pdf dokumentace.ps
# nesmaze docasne soubory

dokumentace.ps: $(PROJECT).dvi
	$(DVI2PS) $(DVI2PS_FLAGS) $(PROJECT)

dokumentace.pdf: $(PROJECT).dvi
	$(DVI2PDF) $(DVI2PDF_FLAGS) $(PROJECT).dvi dokumentace.pdf

$(PROJECT).dvi: $(PROJECT).tex $(PROJECT).sty $(SRC)
	$(LATEX) $(LATEX_FLAGS) $(PROJECT)
	$(LATEX) $(LATEX_FLAGS) $(PROJECT)

clean:
	rm -f *.dvi *.log  *.aux
	rm -f $(PROJECT).blg $(PROJECT).bbl $(PROJECT).toc $(PROJECT).out $(PROJECT).zip
	rm -f $(PROJECT).pdf $(PROJECT).ps dokumentace.pdf dokumentace.ps

pack:
	zip $(PROJECT).zip *.tex *.cls *.eps *.sty Makefile
