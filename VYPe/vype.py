#!/usr/bin/env python
# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Main module."""

from src.gui.VYPe2009 import VYPe2009

if __name__ == "__main__":
    VYPe2009 = VYPe2009(0)
    VYPe2009.MainLoop()
