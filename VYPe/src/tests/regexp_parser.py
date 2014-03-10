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
from .. import regexp_parser as parser


class RegExpParserTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def testParseEmptyString(self):
		try:
			parser.RegExpParser.parseRegExp("")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParsePlus(self):
		try:
			parser.RegExpParser.parseRegExp("+")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseStar(self):
		try:
			parser.RegExpParser.parseRegExp("*")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseIncorrect(self):
		try:
			parser.RegExpParser.parseRegExp("a+*b")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseIncorrect(self):
		try:
			parser.RegExpParser.parseRegExp("a(+b)")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseIncorrectCloseBracket(self):
		try:
			parser.RegExpParser.parseRegExp("a+)(b)")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseIncorrectCloseBracket2(self):
		try:
			parser.RegExpParser.parseRegExp("(b")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseUnknownEscapeSequence(self):
		try:
			parser.RegExpParser.parseRegExp("\\b")
		except ValueError, e:
			pass
		else:
			self.fail("Exception not thrown")

	def testParseES(self):
		tree = parser.RegExpParser.parseRegExp(r"\0")
		reference = r.EmptySetRegExp()
		self.assertEqual(tree, reference)

	def testParseEps(self):
		tree = parser.RegExpParser.parseRegExp(r"\e")
		reference = r.EpsilonRegExp()
		self.assertEqual(tree, reference)

	def testParseSymbol(self):
		tree = parser.RegExpParser.parseRegExp("a")
		reference = r.SingleSymbolRegExp("a")
		self.assertEqual(tree, reference)

	def testParseConcat2(self):
		tree = parser.RegExpParser.parseRegExp("ab")
		reference = r.ConcatRegExp(r.SingleSymbolRegExp("a"),
				r.SingleSymbolRegExp("b"))
		self.assertEqual(tree, reference)

	def testParseConcat3(self):
		tree = parser.RegExpParser.parseRegExp("abc")
		reference = r.ConcatRegExp(r.ConcatRegExp(r.SingleSymbolRegExp("a"),
				r.SingleSymbolRegExp("b")), r.SingleSymbolRegExp("c"))
		self.assertEqual(tree, reference)

	def testParseUnion2(self):
		tree = parser.RegExpParser.parseRegExp("a+b")
		reference = r.UnionRegExp(r.SingleSymbolRegExp("a"),
				r.SingleSymbolRegExp("b"))
		self.assertEqual(tree, reference)

	def testParseUnion3(self):
		tree = parser.RegExpParser.parseRegExp("a+b+c")
		reference = r.UnionRegExp(r.UnionRegExp(r.SingleSymbolRegExp("a"),
				r.SingleSymbolRegExp("b")), r.SingleSymbolRegExp("c"))
		self.assertEqual(tree, reference)

	def testParseIter(self):
		tree = parser.RegExpParser.parseRegExp("a*")
		reference = r.IterRegExp(r.SingleSymbolRegExp("a"))
		self.assertEqual(tree, reference)

	def testParseIter2(self):
		tree = parser.RegExpParser.parseRegExp("a**")
		reference = r.IterRegExp(r.IterRegExp(r.SingleSymbolRegExp("a")))
		self.assertEqual(tree, reference)

	def testParseBrackets(self):
		tree = parser.RegExpParser.parseRegExp("(a)")
		reference = r.BracketsRegExp(r.SingleSymbolRegExp("a"))
		self.assertEqual(tree, reference)

	def testParsePriorityConcatIter(self):
		tree = parser.RegExpParser.parseRegExp("ab*")
		reference = r.ConcatRegExp(r.SingleSymbolRegExp("a"),
			r.IterRegExp(r.SingleSymbolRegExp("b")))
		self.assertEqual(tree, reference)

	def testParsePriorityShort(self):
		tree = parser.RegExpParser.parseRegExp("a+ab*")
		reference = r.UnionRegExp(r.SingleSymbolRegExp("a"),
					r.ConcatRegExp(r.SingleSymbolRegExp("a"),
						r.IterRegExp(r.SingleSymbolRegExp("b"))))
		self.assertEqual(tree, reference)

	def testParsePriority(self):
		tree = parser.RegExpParser.parseRegExp("a+ab*+(a+a)b*")
		reference = r.UnionRegExp(
				r.UnionRegExp(r.SingleSymbolRegExp("a"),
					r.ConcatRegExp(r.SingleSymbolRegExp("a"),
						r.IterRegExp(r.SingleSymbolRegExp("b")))),
				r.ConcatRegExp(
					r.BracketsRegExp(r.UnionRegExp(
						r.SingleSymbolRegExp("a"), r.SingleSymbolRegExp("a"))),
					r.IterRegExp(r.SingleSymbolRegExp("b"))))
		self.assertEqual(tree, reference)

	def testRepr(self):
		re = "a+ab*+(a+a)b*+\\e+\\0+\\+\\*\\(\\)+\\\\"
		tree = parser.RegExpParser.parseRegExp(re)
		self.assertEqual(unicode(tree), re)

	def testUnicode(self):
		tree = parser.RegExpParser.parseRegExp(u"š")
		self.assertTrue(isinstance(unicode(tree), unicode))


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
