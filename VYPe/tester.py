#!/usr/bin/env python
# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Runs all project unit tests."""

import unittest

import src.tests


def main():
	"""Runs all project unit tests."""
	# Gather all tests
	allTests = [src.tests.suite()]

	# Run them
	runner = unittest.TextTestRunner()
	runner.run(unittest.TestSuite(allTests))


if __name__ == '__main__':
	main()
