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

from .. import nfas


class StateTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def testTwoDifferentStatesHaveDifferentNamesByDefault(self):
		s1 = nfas.State()
		s2 = nfas.State()
		self.assertNotEqual(s1, s2)

	def testTwoStatesWithSameNameAndStartingAndFinalAreSame(self):
		s1 = nfas.State(name=u"A")
		s2 = nfas.State(name=u"A")
		self.assertEqual(s1, s2)

	def testTwoStatesWithDifferentNameAndStartingAndFinalAreNotSame(self):
		s1 = nfas.State(name=u"A")
		s2 = nfas.State(name=u"B")
		self.assertNotEqual(s1, s2)

	def testTwoStatesWithDifferentStartingStatusAreNotSame(self):
		s1 = nfas.State(isStarting=True)
		s2 = nfas.State(isStarting=False)
		self.assertNotEqual(s1, s2)

	def testTwoStatesWithDifferentFinalStatusAreNotSame(self):
		s1 = nfas.State(isFinal=True)
		s2 = nfas.State(isFinal=False)
		self.assertNotEqual(s1, s2)

	def testTwoStatesWithDifferentWasChangedStatusAreNotSame(self):
		s1 = nfas.State(wasChanged=True)
		s2 = nfas.State(wasChanged=False)
		self.assertNotEqual(s1, s2)

	def testTwoDifferentStatesAreNotEqual(self):
		self.assertNotEqual(nfas.State(), nfas.State())

	def testTwoSameStatesAreEqual(self):
		s = nfas.State()
		self.assertEqual(s, s)

	def testStrReturnsTheSameAsGetName(self):
		s = nfas.State()
		self.assertEqual(s.getName(), s.__str__())

	def testUnicodeReturnsTheSameAsGetName(self):
		s = nfas.State()
		self.assertEqual(s.getName(), s.__unicode__())


class NFATests(unittest.TestCase):
	"""Base class for all NFA subclasses tests. All NFA subclasses test cases
	should inherit from this class and have self.a attribute set to the NFA
	instance under test, because methods of this class will test NFA class
	(and thus subclass) invariants."""
	def setUp(self):
		# Create a NFA stub so tests in this class will pass
		class NFAStub(nfas.NFA):
			def __init__(self):
				self.__startingState = nfas.State(isStarting=True)
			def getStates(self):
				return [self.__startingState]
			def getStartingState(self):
				return self.__startingState
			def getFinalStates(self):
				return []
			def getTransitions(self):
				return []
		self.a = NFAStub()

	def tearDown(self):
		pass

	def testStartingStateIsStartingState(self):
		self.assertTrue(self.a.getStartingState().isStarting())

	def testHasOnlyOneStartingState(self):
		numOfStartingStates = 0
		for state in self.a.getStates():
			if state.isStarting():
				numOfStartingStates += 1

		self.assertEqual(1, numOfStartingStates)

	def testIfThereIsAFinalStateItIsFinalState(self):
		fStates = self.a.getFinalStates()
		if len(fStates) > 0:
			self.assertTrue(fStates[0].isFinal())

	def testHasAtMostOneFinalState(self):
		finalStatesCnt = 0
		for state in self.a.getStates():
			if state.isFinal():
				finalStatesCnt += 1

		self.assertTrue(0 <= finalStatesCnt <= 1)

	def testGetStartingStateAlwaysReturnsTheSameState(self):
		firstCallStartingState = self.a.getStartingState()
		self.assertEqual(firstCallStartingState, self.a.getStartingState())

	def testGetStatesAlwaysReturnsTheSameStates(self):
		firstCallStates = self.a.getStates()
		for state in self.a.getStates():
			self.assertTrue(state in firstCallStates)

	def testGetFinalStatesAlwaysReturnsTheSameStates(self):
		firstCallFinalStates = self.a.getFinalStates()
		for state in self.a.getFinalStates():
			self.assertTrue(state in firstCallFinalStates)

	def testGetTransitionsAlwaysReturnsTheSameTransitions(self):
		firstCallTransitions = self.a.getTransitions()
		for state in self.a.getTransitions():
			self.assertTrue(state in firstCallTransitions)

	def testIsUmmutableGetStates(self):
		states = self.a.getStates()
		states.append(nfas.State())
		self.assertNotEqual(len(self.a.getStates()), len(states))

	def testIsUmmutableGetFinalStates(self):
		fStates = self.a.getFinalStates()
		fStates.append(nfas.State())
		self.assertNotEqual(len(self.a.getFinalStates()), len(fStates))

	def testIsUmmutableGetTransitions(self):
		trans = self.a.getFinalStates()
		trans.append((nfas.State(), u'', nfas.State()))
		self.assertNotEqual(len(self.a.getFinalStates()), len(trans))


class EmptyNFATests(NFATests):
	def setUp(self):
		self.a = nfas.EmptySetNFA()

	def tearDown(self):
		pass

	def testHasOneState(self):
		self.assertEqual(1, len(self.a.getStates()))

	def testOnlyStateIsStarting(self):
		self.assertTrue(self.a.getStates()[0].isStarting())

	def testStartingStateIsMarkedAsChanged(self):
		self.assertTrue(self.a.getStartingState().wasChanged())

	def testGetFinalStatesReturnsNoFinalStates(self):
		self.assertEqual(0, len(self.a.getFinalStates()))

	def testHasNoFinalStates(self):
		for state in self.a.getStates():
			self.assertTrue(not state.isFinal())

	def testHasNoTransitions(self):
		self.assertEqual([], self.a.getTransitions())


class SingleSymbolStringNFATests(NFATests):
	def setUp(self):
		self.symbol = u'č'
		self.a = nfas.SingleSymbolNFA(self.symbol)

	def tearDown(self):
		pass

	def testHasTwoStates(self):
		self.assertEqual(2, len(self.a.getStates()))

	def testGetFinalStatesReturnsOneFinalState(self):
		self.assertEqual(1, len(self.a.getFinalStates()))

	def testResultHasOneFinalState(self):
		finalStatesCnt = 0
		for state in self.a.getStates():
			if state.isFinal():
				finalStatesCnt += 1

		self.assertEqual(1, finalStatesCnt)

	def testStartingAndFinalStateAreMarkedAsChanged(self):
		self.assertTrue(self.a.getStartingState().wasChanged())
		self.assertTrue(self.a.getFinalStates()[0].wasChanged())

	def testHasOneAndProperTransition(self):
		trans = self.a.getTransitions()
		self.assertEqual(1, len(trans))
		self.assertTrue(trans[0][0].isStarting())
		self.assertEquals(self.symbol, trans[0][1])
		self.assertTrue(trans[0][2].isFinal())

	def testStartingStateHasLesserNameThanFinalState(self):
		self.assertTrue(self.a.getStartingState().getName() <
			self.a.getFinalStates()[0].getName())


class ConcatNFATwoEmptySetNFATests(NFATests):
	def setUp(self):
		self.a1 = nfas.EmptySetNFA()
		self.a2 = nfas.EmptySetNFA()
		self.a = nfas.ConcatNFA(self.a1, self.a2)

	def tearDown(self):
		pass

	def testResultHasTwoStates(self):
		self.assertEqual(2, len(self.a.getStates()))

	def testResultGetFinalStatesReturnsNoFinalStates(self):
		self.assertEqual(0, len(self.a.getFinalStates()))

	def testResultHasNoFinalStates(self):
		for state in self.a.getStates():
			self.assertTrue(not state.isFinal())

	def testResultHasNoTransitions(self):
		self.assertEqual(0, len(self.a.getTransitions()))

	def testStatesInResultHaveSameNamesAsInTwoOrigNFAs(self):
		a1SNames = map(lambda s: s.getName(), self.a1.getStates())
		a2SNames = map(lambda s: s.getName(), self.a2.getStates())
		for state in self.a.getStates():
			self.assertTrue(state.getName() in (a1SNames + a2SNames))

	def testOneStateShouldBeMarkedAsChanged(self):
		changedStatesCount = 0
		for state in self.a.getStates():
			if state.wasChanged():
				changedStatesCount += 1
		self.assertEqual(1, changedStatesCount)


class ConcatNFATwoSingleSymbolNFATests(NFATests):
	def setUp(self):
		self.a1Symbol = u''
		self.a2Symbol = u'b'
		self.a1 = nfas.SingleSymbolNFA(self.a1Symbol)
		self.a2 = nfas.SingleSymbolNFA(self.a2Symbol)
		self.a = nfas.ConcatNFA(self.a1, self.a2)

	def tearDown(self):
		pass

	def testHasFourStates(self):
		self.assertEqual(4, len(self.a.getStates()))

	def testGetFinalStatesReturnsOneFinalState(self):
		self.assertEqual(1, len(self.a.getFinalStates()))

	def testHasOneFinalState(self):
		finalStatesCnt = 0
		for state in self.a.getStates():
			if state.isFinal():
				finalStatesCnt += 1

		self.assertEqual(1, finalStatesCnt)

	def testHasThreeTransitions(self):
		self.assertEqual(3, len(self.a.getTransitions()))

	def testHasProperSymbolsInTransitions(self):
		for (fromState, symbol, toState) in self.a.getTransitions():
			if fromState.isStarting():
				self.assertEqual(self.a1Symbol, symbol)
			elif toState.isFinal():
				self.assertEqual(self.a2Symbol, symbol)
			else:
				self.assertEqual(u'', symbol)

	def testNoStateIsFromStateMoreThanOnce(self):
		statesCntMap = {}
		for (fromState, _, _) in self.a.getTransitions():
			if statesCntMap.has_key(fromState):
				statesCntMap[fromState] += 1
			else:
				statesCntMap[fromState] = 1

		for (fromState, _, _) in self.a.getTransitions():
			self.assertEqual(1, statesCntMap[fromState])

	def testNoStateIsToStateMoreThanOnce(self):
		statesCntMap = {}
		for (_, _, toState) in self.a.getTransitions():
			if statesCntMap.has_key(toState):
				statesCntMap[toState] += 1
			else:
				statesCntMap[toState] = 1

		for (_, _, toState) in self.a.getTransitions():
			self.assertEqual(1, statesCntMap[toState])

	def testStatesInResultHaveSameNamesAsInTwoOrigNFAs(self):
		a1SNames = map(lambda s: s.getName(), self.a1.getStates())
		a2SNames = map(lambda s: s.getName(), self.a2.getStates())
		aSNames = map(lambda s: s.getName(), self.a.getStates())
		for stateName in (a1SNames + a2SNames):
			self.assertTrue(stateName in aSNames)

	def testTwoStatesShouldBeMarkedAsChanged(self):
		changedStatesCount = 0
		for state in self.a.getStates():
			if state.wasChanged():
				changedStatesCount += 1
		self.assertEqual(2, changedStatesCount)

	def testStartingAndFinalStatesShouldNotBeMarkedAsChanged(self):
		for state in self.a.getStates():
			if state.isStarting():
				self.assertTrue(not state.wasChanged())
			if state.isFinal():
				self.assertTrue(not state.wasChanged())

		for (fromState, symbol, toState) in self.a.getTransitions():
			if fromState.isStarting():
				self.assertTrue(not fromState.wasChanged())
			if toState.isFinal():
				self.assertTrue(not toState.wasChanged())


class UnionNFATwoSingleSymbolNFATests(NFATests):
	def setUp(self):
		self.a1Symbol = u''
		self.a2Symbol = u'b'
		self.a1 = nfas.SingleSymbolNFA(self.a1Symbol)
		self.a2 = nfas.SingleSymbolNFA(self.a2Symbol)
		self.a = nfas.UnionNFA(self.a1, self.a2)

	def tearDown(self):
		pass

	def testHasSixStates(self):
		self.assertEqual(6, len(self.a.getStates()))

	def testHasOneFinalState(self):
		finalStatesCnt = 0
		for state in self.a.getStates():
			if state.isFinal():
				finalStatesCnt += 1

		self.assertEqual(1, finalStatesCnt)

	def testGetFinalStatesReturnsOneFinalState(self):
		self.assertEqual(1, len(self.a.getFinalStates()))

	def testStartingStateIsSetAsChangedState(self):
		self.assertTrue(self.a.getStartingState().wasChanged())

	def testFinalStateIsSetAsChangedState(self):
		self.assertTrue(self.a.getFinalStates()[0].wasChanged())

	def testHasProperNumberOfTransitions(self):
		self.assertEqual(6, len(self.a.getTransitions()))

	def testHasProperSymbolsInTransitionsAndStates(self):
		for (fromState, symbol, toState) in self.a.getTransitions():
			if fromState.isStarting():
				self.assertEqual(u'', symbol)
				self.assertTrue(toState.getName() == self.a1.getStartingState().getName()
						or toState.getName() == self.a2.getStartingState().getName())
			if toState.isFinal():
				self.assertEqual(u'', symbol)
				self.assertTrue(fromState.getName() == self.a1.getFinalStates()[0].getName()
						or fromState.getName() == self.a2.getFinalStates()[0].getName())
			if fromState.getName() == self.a1.getStartingState().getName():
				self.assertEqual(self.a1Symbol, symbol)
			if fromState.getName() == self.a2.getStartingState().getName():
				self.assertEqual(self.a2Symbol, symbol)

	def testStatesInResultHaveSameNamesAsInTwoOrigNFAs(self):
		a1SNames = map(lambda s: s.getName(), self.a1.getStates())
		a2SNames = map(lambda s: s.getName(), self.a2.getStates())
		aSNames = map(lambda s: s.getName(), self.a.getStates())
		for stateName in (a1SNames + a2SNames):
			self.assertTrue(stateName in aSNames)

	def testSixStatesShouldBeMarkedAsChanged(self):
		changedStatesCount = 0
		for state in self.a.getStates():
			if state.wasChanged():
				changedStatesCount += 1
		self.assertEqual(6, changedStatesCount)


class IterNFASingleSymbolNFATests(NFATests):
	def setUp(self):
		self.oSymbol = u'g'
		self.o = nfas.SingleSymbolNFA(self.oSymbol)
		self.a = nfas.IterNFA(self.o)

	def tearDown(self):
		pass

	def testHasFourStates(self):
		self.assertEqual(4, len(self.a.getStates()))

	def testGetFinalStatesReturnsOneFinalState(self):
		self.assertEqual(1, len(self.a.getFinalStates()))

	def testHasOneFinalState(self):
		finalStatesCnt = 0
		for state in self.a.getStates():
			if state.isFinal():
				finalStatesCnt += 1

		self.assertEqual(1, finalStatesCnt)

	def testStartingStateIsSetAsChangedState(self):
		self.assertTrue(self.a.getStartingState().wasChanged())

	def testFinalStateIsSetAsChangedState(self):
		self.assertTrue(self.a.getFinalStates()[0].wasChanged())

	def testHasProperNumberOfTransitions(self):
		self.assertEqual(5, len(self.a.getTransitions()))

	def testHasProperSymbolsInTransitionsAndStates(self):
		for (fromState, symbol, toState) in self.a.getTransitions():
			if fromState.isStarting():
				self.assertEqual(u'', symbol)
				self.assertTrue(toState.getName() == self.o.getStartingState().getName() or
						toState.getName() == self.a.getFinalStates()[0].getName())
			if toState.isFinal():
				self.assertEqual(u'', symbol)
				self.assertTrue(fromState.getName() == self.o.getFinalStates()[0].getName() or
						fromState.getName() == self.a.getStartingState().getName())
			if fromState.getName() == self.o.getStartingState().getName():
				self.assertEqual(self.oSymbol, symbol)

	def testStatesInResultHaveSameNamesAsInOrigNFA(self):
		oStateNames = map(lambda s: s.getName(), self.o.getStates())
		aStateNames = map(lambda s: s.getName(), self.a.getStates())
		for stateName in oStateNames:
			self.assertTrue(stateName in aStateNames)

	def testSixStatesShouldBeMarkedAsChanged(self):
		changedStatesCount = 0
		for state in self.a.getStates():
			if state.wasChanged():
				changedStatesCount += 1
		self.assertEqual(4, changedStatesCount)


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
