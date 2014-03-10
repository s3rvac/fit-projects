#
# Encoding: utf8
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Usage:
#   - create documentation (pdf):      make
#   - create documentation (ps):       make ps
#   - clean:                           make clean
#

# Documentation file name with path (without extension)
DOCUMENTATION = documentation

# Commands
LATEX   = cslatex
DVI2PDF = dvipdf
DVI2PS  = dvips

# Flags
LATEX_FLAGS   =
DVI2PDF_FLAGS =
DVI2PS_FLAGS  = -o $(DOCUMENTATION).ps


# Default pdf version
all: $(DOCUMENTATION).pdf
	rm -f *.dvi *.log  *.aux

# Postscript version
ps: $(DOCUMENTATION).ps
	rm -f $(SRC)*.dvi $(SRC)*.log  $(SRC)*.aux

# Rules
$(DOCUMENTATION).ps: $(DOCUMENTATION).dvi
	$(DVI2PS) $(DVI2PS_FLAGS) $(DOCUMENTATION)

$(DOCUMENTATION).pdf: $(DOCUMENTATION).dvi
	$(DVI2PDF) $(DVI2PDF_FLAGS) $(DOCUMENTATION).dvi $(DOCUMENTATION).pdf

$(DOCUMENTATION).dvi: $(DOCUMENTATION).tex $(DOCUMENTATION).sty
	$(LATEX) $(LATEX_FLAGS) $(DOCUMENTATION)
	$(LATEX) $(LATEX_FLAGS) $(DOCUMENTATION)

# Cleaning
clean:
	rm -f *.dvi *.log *.aux *.tex.backup *.sty.backup
	rm -f $(DOCUMENTATION).blg $(DOCUMENTATION).bbl $(DOCUMENTATION).toc $(DOCUMENTATION).out $(DOCUMENTATION).zip
	rm -f $(DOCUMENTATION).pdf $(DOCUMENTATION).ps $(DOCUMENTATION).pdf $(DOCUMENTATION).ps
