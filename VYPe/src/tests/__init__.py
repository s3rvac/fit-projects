# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Package containing project unit tests."""

import os
import pkgutil
import unittest


def suite():
	"""Returns a test suite that contains all unit tests from this package."""
	# Get all tests from all modules in this package
	allTests = []
	currPkgPath = os.path.dirname(os.path.abspath(__file__))
	for (_, moduleName, _) in pkgutil.iter_modules([currPkgPath]):
		module = __import__(moduleName, globals(), locals())
		allTests.append(module.suite())

	return unittest.TestSuite(allTests)


def test():
	"""Runs all unit tests from this package."""
	runner = unittest.TextTestRunner()
	runner.run(suite())
