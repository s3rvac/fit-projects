# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""This module contains functions operating on the textual
representation of NFAs (the DOT language).
"""

import re


class NumberGenerator(object):
	"""This class generates numbers. Instance of this class can be passed
	to the nextNameFunc to rename NFA states into a sequence of numbers.
	"""
	def __init__(self, seed=1):
		"""Constructs a new number generator.

		seed - first number to be returned when the __call__() function
		       is called (number)
		"""
		self.nextNum = seed

	def __call__(self):
		"""Returns a new number at each call. If the previously returned number
		was x, then it returns x+1.
		"""
		res = self.nextNum
		self.nextNum += 1
		return res


def renameStates(nfaDotRepr, statesMap, nextNameFunc):
	"""Renames states in the selected NFA representation in a DOT language
	and returns the modified representation.

	nfaDotRepr - textual representation of an NFA in a DOT language
	             (unicode string)
	statesMap - mapping between original state names into new state names
	            (dictionary: string->string)
	nextNameFunc - function which when called returns the next state name
	               to be used when renaming a state which is not in statesMap

	When a state gets renamed, the mapping between the original state name
	and the new name will be stored in statesMap.
	"""
	# Compiles the selected regular expression (string)
	def compRegExp(regExp):
		return re.compile(regExp, re.UNICODE)

	# Checks whether the statesMap has the selected sName and if not,
	# adds the mapping key -> nextNameFunc() into the map
	def checkAndSet(sName, statesMap, nextNameFunc):
		if not statesMap.has_key(sName):
			statesMap[sName] = unicode(nextNameFunc())

	res = u''
	for line in filter(lambda line: line != '', nfaDotRepr.split('\n')):
		# State?
		m = compRegExp(r'^node \[shape = (point|circle|doublecircle)(, color = .+?)?\]; (S?)(.+?);$').match(line)
		if m != None:
			sShape, sColor, sIsStart, sName = m.groups()
			if sColor == None:
				sColor = ''
			checkAndSet(sName, statesMap, nextNameFunc)
			res += u'node [shape = ' + sShape + sColor + u']; ' + sIsStart +\
					statesMap[sName] + u';\n'
			continue

		# Transition?
		m = compRegExp(r'^(S?)(.+?) -> (.+?) \[(.*?)\];$').match(line)
		if m != None:
			sIsStart, sName, oName, transProp = m.groups()
			checkAndSet(sName, statesMap, nextNameFunc)
			res += sIsStart + statesMap[sName] + u' -> ' + statesMap[oName] +\
					u' [' + transProp + u'];\n'
			continue

		# Other lines
		res += line + u'\n'
	return res
