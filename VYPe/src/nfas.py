# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""This package contains classes representing nondeterministic finite
automata (NFAs) for the following types of regular expressions:
    - the empty set
    - the empty string
    - single symbol

It also provides classes to create more complex NFAs:
    - concatenation of two NFAs
    - union of two NFAs
    - iterations of an NFAs
"""


import copy


def _makeAllStatesOrdinary(states, statesMap):
	"""Converts all selected states to ordinary states, i.e. starting
	state will be converted into a non-starting state and final states
	(if any) will be converted into non-final states. Mapping between
	the original state and the new state will be stored into stateMap.
	New state names are the same as the names of original states.
	Returns the list of ordinary states (list of State instances).

	states - list of states to be converted (list of State instances)
	statesMap - mapping between original states and converted states
				(dictionary, where keys are state names and values
				are State instances)
	"""
	convStates = []
	for state in states:
		stateWasChanged = state.isFinal() or state.isStarting()
		statesMap[state.getName()] = State(name=state.getName(),
				wasChanged=stateWasChanged)
		convStates.append(statesMap[state.getName()])
	return convStates


def _generateTransitionsWithStateMapping(transitions, statesMap):
	"""Generates a list of transitions from the selected list of transitions
	so that states that were replaced (are in the statesMap) are replaced
	in the resulting list.

	transitions - list of transitions ([(fromState, symbol, to State)])
	statesMap - mapping between original states and converted states
				(dictionary, where keys are state names and values
				are State instances)
	"""
	# Returns the state to be used in a transition corresponding
	# to the selected state (i.e. if it was replaced with another state,
	# return that replaced state).
	def getStateToUse(state, statesMap):
		if statesMap.has_key(state.getName()):
			return statesMap[state.getName()]
		else:
			return state

	genTrans = []
	for (fromState, symbol, toState) in transitions:
		genTrans.append((getStateToUse(fromState, statesMap), symbol,
				getStateToUse(toState, statesMap)))
	return genTrans


class State(object):
	"""This class represents a single state of an NFA.

	Objects of this class are immutable. Two states are equal if they got
	the same name, same "was changed" status and either both are (non-)starting and
	(non-)final.
	"""
	def __init__(self, name=None, isStarting=False, isFinal=False, wasChanged=False):
		"""Creates a state.

		name - name of the state (unicode string); if None, it is chosen randomly
		isStarting - Is the state a starting state (bool)?
		isFinal - Is the state a final state (bool)?
		wasChanged - Should the state be considered as a "changed state"?

		Name of the state is assigned automatically (each created state
		has an unique name).
		"""
		self.__name = name if name != None else unicode(id(self))
		self.__isStarting = isStarting
		self.__isFinal = isFinal
		self.__wasChanged = wasChanged

	def isStarting(self):
		"""Returns True if this state is a starting state, False otherwise."""
		return self.__isStarting

	def isFinal(self):
		"""Returns True if this state is a final state, False otherwise."""
		return self.__isFinal

	def wasChanged(self):
		"""Returns True if this state is considered as a "changed" state,
		False otherwise.
		"""
		return self.__wasChanged

	def getName(self):
		"""Returns the name of the state (unicode string)."""
		return self.__name

	def __str__(self):
		"""Returns the name of the state (unicode string).

		Result of this method is the same as of the getName() method."""
		return self.getName()

	def __unicode__(self):
		"""Returns the name of the state (unicode string).

		Result of this method is the same as of the getName() method."""
		return self.getName()

	def __eq__(self, other):
		"""Returns True if this object is equal to the other object,
		False otherwise.
		"""
		return self.getName() == other.getName() and\
				self.isStarting() == other.isStarting() and\
				self.isFinal() == other.isFinal() and\
				self.wasChanged() == other.wasChanged()

	def __ne__(self, other):
		"""Returns True if this object is NOT equal to the other object,
		False otherwise.
		"""
		return not (self == other)


class NFA(object):
	"""Abstract class representing a general NFA.

	Requirements for all subclasses:
		- must be immutable
		- an NFA must have one starting state
		- an NFA must have at most one final state

	Limitations:
		- if not explicitely stated by subclasses, you cannot compare two NFAs
	"""

	def __init__(self):
		"""Constructs an abstract NFA.

		This method must be called only from subclasses.
		"""
		if isinstance(self, NFA):
			raise NotImplementedError, "Cannot instantiate abstract class."

	def getStates(self):
		"""Returns a list of all automaton states."""
		raise NotImplementedError, "Abstract method called."

	def getStartingState(self):
		"""Returns the starting state of the automaton."""
		raise NotImplementedError, "Abstract method called."

	def getFinalStates(self):
		"""Returns a list of all automaton states, which are final.

		This must either be an empty list (the NFA has no final states),
		or a list containing one state, which is final.
		"""
		raise NotImplementedError, "Abstract method called."

	def getTransitions(self):
		"""Returns a list of all automaton transitions in the form
		(from state, symbol, to state).
		"""
		raise NotImplementedError, "Abstract method called."


class EmptySetNFA(NFA):
	"""NFA corresponding to the regular expression denoting
	the empty language.
	"""

	def __init__(self):
		"""Constructs an NFA that accepts the empty language.

		It always has the following structure:
		       -----
		    -->| p |
		       -----

		State name is chosen randomly. State is marked as "changed".
		"""
		super(NFA, self).__init__()
		self.__startingState = State(isStarting=True, wasChanged=True)

	def getStates(self):
		"""Override."""
		return [self.__startingState]

	def getStartingState(self):
		"""Override."""
		return self.__startingState

	def getFinalStates(self):
		"""Overrride."""
		return []

	def getTransitions(self):
		"""Override."""
		return []


class SingleSymbolNFA(NFA):
	"""NFA corresponding to the regular expression denoting the language
	that contains only one symbol from the input alphabet or the empty string.
	"""

	def __init__(self, symbol):
		"""Constructs an NFA that accepts only the selected symbol.

		symbol - Symbol to be accepted (unicode string).

		Preconditions:
			- symbol == u'' or len(symbol) == 1

		It always has the following structure (q is a final state):

		       -----   symbol  -------
		    -->| p |---------->|| q ||
		       -----           -------

		State name is chosen randomly. States are marked as "changed".

		Postconditions:
			- Name of the starting state will be lexicographically lesser
			  than the name of the final state.
		"""
		super(NFA, self).__init__()

		# Preconditions
		assert(symbol == u'' or len(symbol) == 1)

		self.__startingState = State(isStarting=True, wasChanged=True)
		self.__finalState = State(isFinal=True, wasChanged=True)

		# Postconditions
		if self.__startingState.getName() > self.__finalState.getName():
			# Switch state names to assure the postcondition
			ssName = self.__startingState.getName()
			fsName = self.__finalState.getName()
			self.__startingState = State(fsName, isStarting=True, wasChanged=True)
			self.__finalState = State(ssName, isFinal=True, wasChanged=True)

		self.__transitions = [(self.__startingState, symbol, self.__finalState)]

	def getStates(self):
		"""Override."""
		return [self.__startingState, self.__finalState]

	def getStartingState(self):
		"""Override."""
		return self.__startingState

	def getFinalStates(self):
		"""Overrride."""
		return [self.__finalState]

	def getTransitions(self):
		"""Override."""
		return self.__transitions


class ConcatNFA(NFA):
	"""NFA representing a concatenation of two NFAs."""

	def __init__(self, nfa1, nfa2):
		"""Constructs an NFA from the two given NFAs that accepts the language
		L(nfa1)L(nfa2) (i.e. concatenation of these two langiages).

		nfa1 - First NFA (NFA instance).
		nfa2 - Second NFA (NFA instance).

		It always has the following structure (q1 was a final state in nfa1,
		q2 will be the final state of the resulting NFA):

		    |-------- nfa1 ---------      |-------- nfa2 --------|
		    |   ------     ------  | u''  |  ------     -------- |
		    |-->| p1 |---->| q1 |--|------|->| p2 |---->|| q2 || |
		    |   ------     ------  |      |  ------     -------- |
		    ------------------------      -----------------------|

		New state names are chosen randomly. Any state changed from
		a starting to a non-starting state and any state changed from
		a final state to a non-final state will be marked as "changed".

		Restrictions:
			- currently, nfa1 must be a different object than nfa2
		"""
		super(NFA, self).__init__()

		# Mapping between states of nfa1 (nfa2) and states in the resulting NFA
		statesMap = {}

		# Starting state of the resulting NFA is the starting state of nfa1
		# (change the status of the state to "non-changed")
		self.__startingState = State(nfa1.getStartingState().getName(),
				isStarting=True)
		# Also add it to the states map to avoid problems when creating
		# transitions
		statesMap[self.__startingState.getName()] = self.__startingState

		# Final state of the resulting NFA is the final state of nfa2 (if any)
		# (change the status of the state to "non-changed")
		self.__finalStates = []
		for state in nfa2.getFinalStates():
			fState = State(state.getName(), isFinal=True)
			self.__finalStates.append(fState)
			# Also add it to the states map to avoid problems when creating
			# transitions
			statesMap[fState.getName()] = fState

		# Generate states
		self.__states = [self.__startingState] + self.__finalStates
		# Change the final state (if any) of the nfa1 into a non-final state
		# and starting state of nfa2 into a non-starting state
		statesToConvert = filter(lambda s: not s.isStarting(), nfa1.getStates()) +\
				filter(lambda s: not s.isFinal(), nfa2.getStates())
		self.__states += _makeAllStatesOrdinary(statesToConvert, statesMap)

		# Generate transitions
		self.__transitions = _generateTransitionsWithStateMapping(
				nfa1.getTransitions() + nfa2.getTransitions(), statesMap)
		# Create the middle epsilon transition from the former final state
		# of nfa1 to the former starting state of nfa2
		# (we can compare the number of final states with 1 since there is
		# a restriction on the maximal number of final states in the abstract
		# class)
		if len(nfa1.getFinalStates()) == 1:
			self.__transitions.append((statesMap[nfa1.getFinalStates()[0].getName()],
				u'', statesMap[nfa2.getStartingState().getName()]))

	def getStates(self):
		"""Override."""
		# Force immutability class invariant
		return copy.copy(self.__states)

	def getStartingState(self):
		"""Override."""
		return self.__startingState

	def getFinalStates(self):
		"""Overrride."""
		# Force immutability class invariant
		return copy.copy(self.__finalStates)

	def getTransitions(self):
		"""Override."""
		# Force immutability class invariant
		return copy.copy(self.__transitions)


class UnionNFA(NFA):
	"""NFA representing a union of two NFAs.
	"""

	def __init__(self, nfa1, nfa2):
		"""Constructs an NFA from the two given NFAs that accepts the language
		L(nfa1) union L(nfa2).

		nfa1 - First NFA (NFA instance).
		nfa2 - Second NFA (NFA instance).

		It always has the following structure (q1 and q2 are final states
		of nfa1 and nfa2, respectively):

		               |-------- nfa1 -----------
		               |   ------     --------  |
		         u'' --|-->| p1 |---->|| q1 ||--|---
		       ----- | |   ------     --------  |  | u''
		    -->| p |-| |-------------------------  |    -------
		       ----- | |-------- nfa2 -----------  |--->|| q ||
		         u'' | |   ------     --------  |  |    -------
		             --|-->| p2 |---->|| q2 ||--|--- u''
		               |   ------     --------  |
		               --------------------------

		New state names are chosen randomly. Any state changed from
		a starting to a non-starting state and any state changed from
		a final state to a non-final state will be marked as "changed".
		New starting and final state will be also marked as "changed".

		Restrictions:
			- currently, nfa1 must be a different object than nfa2
		"""
		super(NFA, self).__init__()

		# Mapping between states of nfa1 (nfa2) and states in the resulting NFA
		statesMap = {}

		# Create new starting and final states
		self.__startingState = State(isStarting=True, wasChanged=True)
		self.__finalStates = [State(isFinal=True, wasChanged=True)]

		# Generate states
		self.__states = [self.__startingState] + self.__finalStates
		# Change all states of both nfa1 and nfa2 to non-final and non-starting
		self.__states += _makeAllStatesOrdinary(nfa1.getStates() + nfa2.getStates(),
				statesMap)

		# Generate transitions
		self.__transitions = _generateTransitionsWithStateMapping(
				nfa1.getTransitions() + nfa2.getTransitions(), statesMap)
		# Create the two transitions from the new starting state into the former
		# nfa1 and nfa2 starting states
		for nfa in [nfa1, nfa2]:
			self.__transitions.append((self.__startingState, u'',
					statesMap[nfa.getStartingState().getName()]))
		# Create the two transitions from former final states of nfa1 and nfa2
		# into the new final state
		for nfa in [nfa1, nfa2]:
			# (we can compare the number of final states with 1 since there is
			# a restriction on the maximal number of final states in the abstract
			# class)
			if len(nfa.getFinalStates()) == 1:
				self.__transitions.append((statesMap[nfa.getFinalStates()[0].getName()],
					u'', self.__finalStates[0]))

	def getStates(self):
		"""Override."""
		# Force immutability class invariant
		return copy.copy(self.__states)

	def getStartingState(self):
		"""Override."""
		return self.__startingState

	def getFinalStates(self):
		"""Overrride."""
		# Force immutability class invariant
		return copy.copy(self.__finalStates)

	def getTransitions(self):
		"""Override."""
		# Force immutability class invariant
		return copy.copy(self.__transitions)


class IterNFA(NFA):
	"""NFA representing an iteration of an NFA."""

	def __init__(self, nfa):
		"""Constructs an NFA from the two given NFAs that accepts
		the language L(nfa)*.

		nfa - NFA (NFA instance).

		It always has the following structure (p was the starting state
		of nfa and q was the final state of nfa):

		                 --------- nfa --------
		       ----- u'' |   -----     -----  | u'' -------
		    -->| o |-----|-->| p |---->| q |--|---->|| r ||
		       -----     |   -----     -----  |     -------
		                 ------^---------|-----
		                      |         |
		                      |   u''   |
		                      -----------

		New state names are chosen randomly. Any state changed from
		a starting to a non-starting state and any state changed from
		a final state to a non-final state will be marked as "changed".
		New starting and final state will be also marked as "changed".
		"""
		super(NFA, self).__init__()

		# Mapping between states of nfa and states in the resulting NFA
		statesMap = {}

		# Create new starting and final states
		self.__startingState = State(isStarting=True, wasChanged=True)
		self.__finalStates = [State(isFinal=True, wasChanged=True)]

		# Generate states
		self.__states = [self.__startingState] + self.__finalStates
		# Change all states of nfa to non-final and non-starting
		self.__states += _makeAllStatesOrdinary(nfa.getStates(), statesMap)

		# Generate transitions
		self.__transitions = _generateTransitionsWithStateMapping(
				nfa.getTransitions(), statesMap)
		# Create a transition from the new starting state into the former
		# nfa starting state
		self.__transitions.append((self.__startingState, u'',
				statesMap[nfa.getStartingState().getName()]))
		# Create a transition from the new starting state into the final state
		# (zero iteration)
		self.__transitions.append((self.__startingState, u'', self.__finalStates[0]))
		# Create a transition from former final state of nfa into the new final state
		# and into the former starting state of nfa (iteration)
		# (we can compare the number of final states with 1 since there is
		# a restriction on the maximal number of final states in the abstract
		# class)
		if len(nfa.getFinalStates()) == 1:
			self.__transitions.append((statesMap[nfa.getFinalStates()[0].getName()],
				u'', self.__finalStates[0]))
			self.__transitions.append((statesMap[nfa.getFinalStates()[0].getName()],
				u'', statesMap[nfa.getStartingState().getName()]))

	def getStates(self):
		"""Override."""
		# Force immutability class invariant
		return copy.copy(self.__states)

	def getStartingState(self):
		"""Override."""
		return self.__startingState

	def getFinalStates(self):
		"""Overrride."""
		# Force immutability class invariant
		return copy.copy(self.__finalStates)

	def getTransitions(self):
		"""Override."""
		# Force immutability class invariant
		return copy.copy(self.__transitions)

