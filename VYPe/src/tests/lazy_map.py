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

from .. import lazy_map as lm

class LazyMapFunc(object):
	"""Function that can be used for testing the lazyMap function
	implementation.
	"""
	def __init__(self):
		self.called = 0
		self.lastItem = None

	def __call__(self, x):
		self.called += 1
		self.lastItem = x


class LazyMapTests(unittest.TestCase):
	def setUp(self):
		self.func = LazyMapFunc()

	def tearDown(self):
		pass

	def testMapOnEmptyListCreatesAnEmptyListAndFuncIsNotCalled(self):
		r = lm.lazyMap(self.func, [])
		self.assertEqual(0, len(r))
		self.assertEqual(0, self.func.called)

	def testMapOnListWithMoreThanOneElementCreatesAnEmptyListAndFuncIsNotCalled(self):
		r = lm.lazyMap(self.func, xrange(1, 4))
		self.assertEqual(0, len(r))
		self.assertEqual(0, self.func.called)

	def testAccessingElementsCausesThemToBeEvaluated(self):
		seq = xrange(0, 11)
		r = lm.lazyMap(self.func, seq)
		for i in seq:
			self.assertEqual(i, len(r))
			self.assertEqual(i, self.func.called)
			try:
				r[i]
			except IndexError, e:
				self.fail('Exception (' + str(e.__class__) + ') should have not been raised.')
			self.assertEqual(i + 1, len(r))
			self.assertEqual(i + 1, self.func.called)
			self.assertEqual(i, self.func.lastItem)

	def testAccessElementInTheMiddleCauseOnlyThatElementToBeEvaluated(self):
		seq = xrange(0, 11)
		r = lm.lazyMap(self.func, seq)
		self.assertEqual(0, len(r))
		self.assertEqual(0, self.func.called)
		try:
			r[5]
		except IndexError, e:
			self.fail('Exception (' + str(e.__class__) + ') should have not been raised.')
		self.assertEqual(1, len(r))
		self.assertEqual(1, self.func.called)
		self.assertEqual(5, self.func.lastItem)

	def testAccessElementForTheSecondTimeDoNotReevaluateIt(self):
		seq = xrange(0, 11)
		r = lm.lazyMap(self.func, seq)
		self.assertEqual(0, len(r))
		self.assertEqual(0, self.func.called)
		for i in xrange(0, 2):
			try:
				r[5]
			except IndexError, e:
				self.fail('Exception (' + str(e.__class__) + ') should have not been raised.')
			self.assertEqual(1, len(r))
			self.assertEqual(1, self.func.called)
			self.assertEqual(5, self.func.lastItem)

	def testAccessNonExistingElementUpperBoundExceededRaisesIndexError(self):
		seq = xrange(0, 11)
		r = lm.lazyMap(self.func, seq)
		try:
			r[100]
		except IndexError, e:
			pass
		else:
			self.fail('IndexError should have been raised.')

	def testAccessNonExistingElementLowerBoundExceededRaisesIndexError(self):
		seq = xrange(0, 11)
		r = lm.lazyMap(self.func, seq)
		try:
			r[-1]
		except IndexError, e:
			pass
		else:
			self.fail('IndexError should have been raised.')


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
