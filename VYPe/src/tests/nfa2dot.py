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

from .. import nfa2dot
from .. import nfas

# Color aliases
NORM_C = nfa2dot.DEF_NORMAL_COLOR
HIGH_C = nfa2dot.DEF_HIGHLIGHT_COLOR

class NFA2DOTTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def scenarioConvertNFAToDOTAndTestOutput(self, nfa, expectedResult):
		result = nfa2dot.convertNFAToDOT(nfa)
		self.assertEqual(expectedResult, result)

	def testConversionOfEmptySetNFA(self):
		nfa = nfas.EmptySetNFA()
		ssName = nfa.getStartingState().getName()
		expectedResult = u"digraph finite_state_machine {\n" +\
			u"node [shape = point, color = " + HIGH_C + u", fontcolor = " + HIGH_C +\
				u"]; S" + ssName + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + u", fontcolor = " + HIGH_C +\
				u"]; " + ssName + u";\n" +\
			u'S' + ssName + ' -> ' + ssName + u' [ label = "", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + u' ];\n' +\
			u"}\n"
		self.scenarioConvertNFAToDOTAndTestOutput(nfa, expectedResult)

	def testConversionOfSingleSymbolNFAEmptyStringAsSymbol(self):
		nfa = nfas.SingleSymbolNFA("")
		ssName = nfa.getStartingState().getName()
		fsName = nfa.getFinalStates()[0].getName()
		expectedResult = u"digraph finite_state_machine {\n" +\
			u"node [shape = point, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; S" + ssName + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + ssName + u";\n" +\
			u"node [shape = doublecircle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + fsName + u";\n" +\
			u'S' + ssName + u' -> ' + ssName + u' [ label = "", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + u' ];\n' +\
			ssName + u' -> ' + fsName + u' [ label = "' +\
				nfa2dot.EMPTY_STRING_TRANS_SYM + u'", fontname = "Times-Italic", ' +\
				'color = ' + HIGH_C + u', fontcolor = ' + HIGH_C + u' ];\n' +\
			u"}\n"
		self.scenarioConvertNFAToDOTAndTestOutput(nfa, expectedResult)

	def testConversionOfSingleSymbolNFAQuoteAsSymbolMustBePrefixed(self):
		nfa = nfas.SingleSymbolNFA('"')
		ssName = nfa.getStartingState().getName()
		fsName = nfa.getFinalStates()[0].getName()
		expectedResult = u"digraph finite_state_machine {\n" +\
			u"node [shape = point, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; S" + ssName + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + ssName + u";\n" +\
			u"node [shape = doublecircle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + fsName + u";\n" +\
			u'S' + ssName + u' -> ' + ssName + u' [ label = "", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + u' ];\n' +\
			ssName + u' -> ' + fsName + u' [ label = "\\\"", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + ' ];\n' +\
			u"}\n"
		self.scenarioConvertNFAToDOTAndTestOutput(nfa, expectedResult)

	def testConversionOfSingleSymbolNFABackslashAsSymbolMustBePrefixed(self):
		nfa = nfas.SingleSymbolNFA('\\')
		ssName = nfa.getStartingState().getName()
		fsName = nfa.getFinalStates()[0].getName()
		expectedResult = u"digraph finite_state_machine {\n" +\
			u"node [shape = point, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; S" + ssName + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + ssName + u";\n" +\
			u"node [shape = doublecircle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + fsName + u";\n" +\
			u'S' + ssName + u' -> ' + ssName + u' [ label = "", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + u' ];\n' +\
			ssName + u' -> ' + fsName + u' [ label = "\\\\", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + ' ];\n' +\
			u"}\n"
		self.scenarioConvertNFAToDOTAndTestOutput(nfa, expectedResult)

	def testConversionOfSingleSymbolNFANonEmptyStringAsSymbol(self):
		nfa = nfas.SingleSymbolNFA('T')
		ssName = nfa.getStartingState().getName()
		fsName = nfa.getFinalStates()[0].getName()
		expectedResult = u"digraph finite_state_machine {\n" +\
			u"node [shape = point, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; S" + ssName + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + ssName + u";\n" +\
			u"node [shape = doublecircle, color = " + HIGH_C + u', fontcolor = ' + HIGH_C +\
				u"]; " + fsName + u";\n" +\
			u'S' + ssName + u' -> ' + ssName + u' [ label = "", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + u' ];\n' +\
			ssName + u' -> ' + fsName + u' [ label = "T", color = ' + HIGH_C +\
				u', fontcolor = ' + HIGH_C + ' ];\n' +\
			u"}\n"
		self.scenarioConvertNFAToDOTAndTestOutput(nfa, expectedResult)

	def testConversionOfConcatNFA(self):
		nfa = nfas.ConcatNFA(nfas.SingleSymbolNFA("A"), nfas.SingleSymbolNFA("B"))
		ssName = nfa.getStartingState().getName()
		m1Name = nfa.getTransitions()[2][0].getName()
		m2Name = nfa.getTransitions()[2][2].getName()
		fsName = nfa.getFinalStates()[0].getName()
		expectedResult = u"digraph finite_state_machine {\n" +\
			u"node [shape = point, color = " + NORM_C + u', fontcolor = ' + NORM_C +\
				u"]; S" + ssName + u";\n" +\
			u"node [shape = circle, color = " + NORM_C + u', fontcolor = ' + NORM_C +\
				u"]; " + ssName + u";\n" +\
			u"node [shape = doublecircle, color = " + NORM_C + u', fontcolor = ' + NORM_C +\
				u"]; " + fsName + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + ', fontcolor = ' + HIGH_C +\
				u"]; " + m1Name + u";\n" +\
			u"node [shape = circle, color = " + HIGH_C + ', fontcolor = ' + HIGH_C +\
				u"]; " + m2Name + u";\n" +\
			u'S' + ssName + ' -> ' + ssName + u' [ label = "", color = ' + NORM_C +\
				u', fontcolor = ' + NORM_C + u' ];\n' +\
			ssName + u' -> ' + m1Name + u' [ label = "A", color = ' + NORM_C +\
				u', fontcolor = ' + NORM_C + u' ];\n' +\
			m2Name + u' -> ' + fsName + u' [ label = "B", color = ' + NORM_C +\
				u', fontcolor = ' + NORM_C + u' ];\n' +\
			m1Name + u' -> ' + m2Name + u' [ label = "' +\
				nfa2dot.EMPTY_STRING_TRANS_SYM + u'", fontname = "Times-Italic", color = ' +\
				HIGH_C + u', fontcolor = ' + HIGH_C + u' ];\n' +\
			u"}\n"
		self.scenarioConvertNFAToDOTAndTestOutput(nfa, expectedResult)


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
