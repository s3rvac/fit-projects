#
# Makefile - GNU Make
# Project: FAV 2009
# Authors:
#     Ondrej Lengal, xlenga00@stud.fit.vutbr.cz
#     Libor Polcak, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - create documentation (pdf):      make
#   - clean:                           make clean
#

# Project
PROJECT = fav

# Commands
LATEX   = pdflatex

# Flags
LATEX_FLAGS   =


# Default pdf version
all: $(PROJECT).pdf

$(PROJECT).pdf: $(wildcard *.tex) $(PROJECT).sty
	$(LATEX) $(LATEX_FLAGS) $(PROJECT)
	$(LATEX) $(LATEX_FLAGS) $(PROJECT)


# Cleaning
clean:
	rm -f *.log *.aux *.tex.backup *.sty.backup
	rm -f $(PROJECT).blg $(PROJECT).bbl $(PROJECT).toc $(PROJECT).out $(PROJECT).zip $(PROJECT).pdf
