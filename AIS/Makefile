#
# Makefile: GNU - g++
# Encoding: utf8
# Project: AIS - Exhibition ground - IS analysis and design
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Boris Procházka, xproch63@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Usage:
#   - create documentation (pdf):      make
#   - create documentation (ps):       make ps
#   - clean:                           make clean
#

# Project name
PROJECT = ais

# Commands
LATEX   = cslatex
DVI2PDF = dvipdf
DVI2PS  = dvips

# Flags
LATEX_FLAGS   =
DVI2PDF_FLAGS =
DVI2PS_FLAGS  = -o $(PROJECT).ps


# Default pdf version
all: $(PROJECT).pdf
	rm -f *.dvi *.log  *.aux

# Postscript version
ps: $(PROJECT).ps
	rm -f $(SRC)*.dvi $(SRC)*.log  $(SRC)*.aux

# Rules
$(PROJECT).ps: $(PROJECT).dvi
	$(DVI2PS) $(DVI2PS_FLAGS) $(PROJECT)

$(PROJECT).pdf: $(PROJECT).dvi
	$(DVI2PDF) $(DVI2PDF_FLAGS) $(PROJECT).dvi $(PROJECT).pdf

$(PROJECT).dvi: $(PROJECT).tex $(PROJECT).sty
	$(LATEX) $(LATEX_FLAGS) $(PROJECT)
	$(LATEX) $(LATEX_FLAGS) $(PROJECT)

# Cleaning
clean:
	rm -f *.dvi *.log *.aux *.tex.backup *.sty.backup
	rm -f $(PROJECT).blg $(PROJECT).bbl $(PROJECT).toc $(PROJECT).out $(PROJECT).zip
	rm -f $(PROJECT).pdf $(PROJECT).ps $(PROJECT).pdf $(PROJECT).ps
