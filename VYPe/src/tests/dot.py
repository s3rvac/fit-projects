# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

import unittest
import sys

from .. import dot


class NumberGeneratorTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def testNumbersGetsProperlyGeneratedWithDefaultSeed(self):
		g = dot.NumberGenerator()
		for i in xrange(1, 12):
			self.assertEqual(i, g())

	def testNumbersGetsProperlyGeneratedWithDefaultCustomSeed(self):
		seed = 50
		g = dot.NumberGenerator(seed)
		for i in xrange(seed, seed + 25):
			self.assertEqual(i, g())

	def testNumbersGetsProperlyGeneratedWhenUsingFloatNumbers(self):
		g = dot.NumberGenerator(1.0)
		i = 1.0
		while i < 15.0:
			self.assertEqual(i, g())
			i += 1

	def testNumbersGetsProperlyGeneratedWhenSeedIsMaxInt(self):
		seed = sys.maxint
		g = dot.NumberGenerator(seed)
		i = seed
		while i < (seed + 25):
			self.assertEqual(i, g())
			i += 1


class RenameStatesTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def scenarioTestRenameStates(self, origRepr, origStatesMap, nextNameFunc,
			refRepr, refStatesMap):
		statesMap = origStatesMap
		resRepr = dot.renameStates(origRepr, statesMap, nextNameFunc)
		self.assertEqual(refRepr, resRepr)
		self.assertEqual(refStatesMap, statesMap)

	def testTwoStateNFANoPreviouslyRenamedStates(self):
		origRepr = u"digraph finite_state_machine {\n" +\
			u"rankdir=LR;\n" +\
			u"node [shape = point]; S45667;\n" +\
			u"node [shape = circle, color = black]; 45667;\n" +\
			u"node [shape = doublecircle, color = black]; 45668;\n" +\
			u'S45667 -> 45667 [ label = "" ];\n' +\
			u'45667 -> 45668 [ label = "G", color = black, fontcolor = black ];\n' +\
			u"}\n"
		origStatesMap = {}
		nextNameFunc = dot.NumberGenerator()
		refRepr = u"digraph finite_state_machine {\n" +\
			u"rankdir=LR;\n" +\
			u"node [shape = point]; S1;\n" +\
			u"node [shape = circle, color = black]; 1;\n" +\
			u"node [shape = doublecircle, color = black]; 2;\n" +\
			u'S1 -> 1 [ label = "" ];\n' +\
			u'1 -> 2 [ label = "G", color = black, fontcolor = black ];\n' +\
			u"}\n"
		refStatesMap = {u'45667': u'1', u'45668': u'2'}
		self.scenarioTestRenameStates(origRepr, origStatesMap, nextNameFunc,
				refRepr, refStatesMap)

	def testTwoStateNFAOnePreviouslyRenamedState(self):
		origRepr = u"digraph finite_state_machine {\n" +\
			u"rankdir=LR;\n" +\
			u"node [shape = point]; S45667;\n" +\
			u"node [shape = circle, color = black]; 45667;\n" +\
			u"node [shape = doublecircle, color = black]; 45668;\n" +\
			u'S45667 -> 45667 [ label = "" ];\n' +\
			u'45667 -> 45668 [ label = "ů", color = black, fontcolor = black ];\n' +\
			u"}\n"
		origStatesMap = {u'45667': u'5'}
		nextNameFunc = dot.NumberGenerator(6)
		refRepr = u"digraph finite_state_machine {\n" +\
			u"rankdir=LR;\n" +\
			u"node [shape = point]; S5;\n" +\
			u"node [shape = circle, color = black]; 5;\n" +\
			u"node [shape = doublecircle, color = black]; 6;\n" +\
			u'S5 -> 5 [ label = "" ];\n' +\
			u'5 -> 6 [ label = "ů", color = black, fontcolor = black ];\n' +\
			u"}\n"
		refStatesMap = {u'45667': u'5', u'45668': u'6'}
		self.scenarioTestRenameStates(origRepr, origStatesMap, nextNameFunc,
				refRepr, refStatesMap)

	def testThreeStateNFALoopTransition(self):
		origRepr = u"digraph finite_state_machine {\n" +\
			u"rankdir=LR;\n" +\
			u"node [shape = point]; S45667;\n" +\
			u"node [shape = circle, color = black]; 45667;\n" +\
			u"node [shape = circle, color = black]; 45668;\n" +\
			u"node [shape = doublecircle, color = black]; 45669;\n" +\
			u'S45667 -> 45667 [ label = "" ];\n' +\
			u'45667 -> 45668 [ label = "A", color = black, fontcolor = black ];\n' +\
			u'45668 -> 45669 [ label = "B", color = black, fontcolor = black ];\n' +\
			u'45669 -> 45669 [ label = "C", color = black, fontcolor = black ];\n' +\
			u"}\n"
		origStatesMap = {}
		nextNameFunc = dot.NumberGenerator()
		refRepr = u"digraph finite_state_machine {\n" +\
			u"rankdir=LR;\n" +\
			u"node [shape = point]; S1;\n" +\
			u"node [shape = circle, color = black]; 1;\n" +\
			u"node [shape = circle, color = black]; 2;\n" +\
			u"node [shape = doublecircle, color = black]; 3;\n" +\
			u'S1 -> 1 [ label = "" ];\n' +\
			u'1 -> 2 [ label = "A", color = black, fontcolor = black ];\n' +\
			u'2 -> 3 [ label = "B", color = black, fontcolor = black ];\n' +\
			u'3 -> 3 [ label = "C", color = black, fontcolor = black ];\n' +\
			u"}\n"
		refStatesMap = {u'45667': u'1', u'45668': u'2', u'45669': u'3'}
		self.scenarioTestRenameStates(origRepr, origStatesMap, nextNameFunc,
				refRepr, refStatesMap)


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
