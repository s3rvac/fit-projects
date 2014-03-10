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

from .. import regexp as r


class RegExpTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def testCreateAbstractClass(self):
		try:
			a = r.RegExp()
		except NotImplementedError:
			pass
		else:
			self.fail("NotImplementedError not thrown")

	def testCreateEmptySet(self):
		a = r.EmptySetRegExp()

	def testCreateEpsilon(self):
		a = r.EpsilonRegExp()

	def testCreateEmptySingleSymbol(self):
		try:
			a = r.SingleSymbolRegExp(None)
		except AssertionError:
			pass
		else:
			self.fail("Assertion not thrown")

	def testCreateLongSingleSymbol(self):
		try:
			a = r.SingleSymbolRegExp("Bad")
		except AssertionError:
			pass
		else:
			self.fail("Assertion not thrown")

	def testESEquality(self):
		a = r.EmptySetRegExp()
		b = r.EmptySetRegExp()
		self.assertTrue(a == b)
		self.assertFalse(a != b)

	def testESNonEquality(self):
		a = r.EmptySetRegExp()
		b = r.EpsilonRegExp()
		self.assertFalse(a == b)
		self.assertTrue(a != b)

	def testEpsEquality(self):
		a = r.EpsilonRegExp()
		b = r.EpsilonRegExp()
		self.assertTrue(a == b)
		self.assertFalse(a != b)

	def testEpsNonEquality(self):
		a = r.EpsilonRegExp()
		b = r.EmptySetRegExp()
		self.assertFalse(a == b)
		self.assertTrue(a != b)

	def testSSEquality(self):
		a = r.SingleSymbolRegExp("a")
		b = r.SingleSymbolRegExp("a")
		self.assertTrue(a == b)
		self.assertFalse(a != b)

	def testSSNonEquality(self):
		a = r.SingleSymbolRegExp("a")
		b = r.SingleSymbolRegExp("b")
		self.assertFalse(a == b)
		self.assertTrue(a != b)

	def testUnionEquality(self):
		u = r.SingleSymbolRegExp("a")
		v = r.SingleSymbolRegExp("a")
		a = r.UnionRegExp(u, v)
		b = r.UnionRegExp(u, v)
		self.assertTrue(a == b)
		self.assertFalse(a != b)

	def testUnionNonEquality(self):
		u = r.SingleSymbolRegExp("a")
		v = r.SingleSymbolRegExp("b")
		a = r.UnionRegExp(u, v)
		b = r.UnionRegExp(v, u)
		self.assertFalse(a == b)
		self.assertTrue(a != b)

	def testConcatEquality(self):
		u = r.SingleSymbolRegExp("a")
		v = r.SingleSymbolRegExp("a")
		a = r.ConcatRegExp(u, v)
		b = r.ConcatRegExp(u, v)
		self.assertTrue(a == b)
		self.assertFalse(a != b)

	def testConcatNonEquality(self):
		u = r.SingleSymbolRegExp("a")
		v = r.SingleSymbolRegExp("b")
		a = r.ConcatRegExp(u, v)
		b = r.ConcatRegExp(v, u)
		self.assertFalse(a == b)
		self.assertTrue(a != b)

	def testIterEquality(self):
		u = r.SingleSymbolRegExp("a")
		a = r.IterRegExp(u)
		b = r.IterRegExp(u)
		self.assertTrue(a == b)
		self.assertFalse(a != b)

	def testIterNonEquality(self):
		u = r.SingleSymbolRegExp("a")
		v = r.SingleSymbolRegExp("b")
		a = r.IterRegExp(u)
		b = r.IterRegExp(v)
		self.assertFalse(a == b)
		self.assertTrue(a != b)


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
