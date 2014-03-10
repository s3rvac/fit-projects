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

from ..tree2nfa import tree2nfa
from .. import regexp as r
from .. import nfas as n

class TmpTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def testTransformEmptySet(self):
		t = r.EmptySetRegExp()
		l = tree2nfa(t)
		self.assertEqual(1, len(l))
		nfa, re = l[0]
		self.assertEqual(nfa.__class__, n.EmptySetNFA)
		self.assertEqual(re, t)

	def testTransformEpsilon(self):
		t = r.EpsilonRegExp()
		l = tree2nfa(t)
		self.assertEqual(1, len(l))
		nfa, re = l[0]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, t)
		self.assertEqual(nfa.getTransitions()[0][1], "")

	def testTransformSingleSymbol(self):
		for i in range(65000, 65535):
			s = unichr(i)
			t = r.SingleSymbolRegExp(s)
			l = tree2nfa(t)
			self.assertEqual(1, len(l))
			nfa, re = l[0]
			self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
			self.assertEqual(re, t)
			self.assertEqual(nfa.getTransitions()[0][1], s)

	def testTransformUnion(self):
		a = "l"
		b = "r"
		rel = r.SingleSymbolRegExp(a)
		rer = r.SingleSymbolRegExp(b)
		t = r.UnionRegExp(rel, rer)
		l = tree2nfa(t)
		self.assertEqual(3, len(l))
		nfa, re = l[0]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, rel)
		nfa, re = l[1]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, rer)
		nfa, re = l[2]
		self.assertEqual(nfa.__class__, n.UnionNFA)
		self.assertEqual(re, t)

	def testTransformConcat(self):
		a = "l"
		b = "r"
		rel = r.SingleSymbolRegExp(a)
		rer = r.SingleSymbolRegExp(b)
		t = r.ConcatRegExp(rel, rer)
		l = tree2nfa(t)
		self.assertEqual(3, len(l))
		nfa, re = l[0]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, rel)
		nfa, re = l[1]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, rer)
		nfa, re = l[2]
		self.assertEqual(nfa.__class__, n.ConcatNFA)
		self.assertEqual(re, t)

	def testTransformIter(self):
		s = "a"
		x = r.SingleSymbolRegExp(s)
		t = r.IterRegExp(x)
		l = tree2nfa(t)
		self.assertEqual(2, len(l))
		nfa, re = l[0]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, x)
		nfa, re = l[1]
		self.assertEqual(nfa.__class__, n.IterNFA)
		self.assertEqual(re, t)

	def testTransformBrackets(self):
		s = "a"
		x = r.SingleSymbolRegExp(s)
		t = r.BracketsRegExp(x)
		l = tree2nfa(t)
		self.assertEqual(2, len(l))
		self.assertEqual(l[0][1], x)
		nfa, re = l[1]
		self.assertEqual(nfa.__class__, n.SingleSymbolNFA)
		self.assertEqual(re, t)
		self.assertEqual(nfa.getTransitions()[0][1], s)


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
