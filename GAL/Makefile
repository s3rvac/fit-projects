#
# Makefile - GNU Make
# Project: GAL 2009
# Authors:
#     Radim Kapavik, xkapav01@stud.fit.vutbr.cz
#     Ondrej Lengal, xlenga00@stud.fit.vutbr.cz
#     Vojtech Storek, xstore02@stud.fit.vutbr.cz
#     Vit Triska, xtrisk01@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Usage:
#   - create documentation (pdf):      make
#   - clean:                           make clean
#

# Project
PROJECT = gal

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
	rm -f $(PROJECT).blg $(PROJECT).bbl $(PROJECT).toc $(PROJECT).out $(PROJECT).zip
