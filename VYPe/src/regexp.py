# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Regular expression handling."""

class RegExp(object):
	"""Abstract base class of the regular expresion."""

	def __init__(self):
		"""Default constructor."""
		if self.__class__ == RegExp:
			raise NotImplementedError("You have tried to construct abstract class object")

	def __eq__(self, other):
		"""Compares two trees."""
		raise NotImplementedError("Abstract method called")

	def __ne__(self, other):
		"""Compares two trees."""
		return not (self == other)

	def __unicode__(self):
		"""Creates machine readable representation of the instance."""
		return object.__unicode__(self)

class EmptySetRegExp(RegExp):
	"""Represents empty language"""

	def __init__(self):
		"""Constructor."""
		RegExp.__init__(self)

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, EmptySetRegExp)

	def __unicode__(self):
		"""Overrides base class method."""
		return "\\0"

class EpsilonRegExp(RegExp):
	"""Represents empty string"""

	def __init__(self):
		"""Constructor."""
		RegExp.__init__(self)

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, EpsilonRegExp)

	def __unicode__(self):
		"""Overrides base class method."""
		return "\\e"

class SingleSymbolRegExp(RegExp):
	"""Represent one alphabet symbol."""

	def __init__(self, sym):
		"""Constructor.

		sym - Symbol that the object represents.
		"""
		assert isinstance(sym, basestring), sym.__class__
		assert len(sym) == 1, len(sym)
		RegExp.__init__(self)
		self.__symbol = sym

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, SingleSymbolRegExp) and \
				self.__symbol == other.getSymbol()

	def getSymbol(self):
		"""Returns represented symbol."""
		return self.__symbol

	def __unicode__(self):
		"""Overrides base class method."""
		if self.__symbol in ["\\", "+", "*", "(", ")"]:
			return "\\" + self.__symbol
		else:
			return self.__symbol

class UnionRegExp(RegExp):
	"""Represents union of two regexps."""

	def __init__(self, left, right):
		"""Constructor.

		left - First regexp
		right - Second regexp
		"""
		assert isinstance(left, RegExp), left.__class__
		assert isinstance(right, RegExp), right.__class__
		RegExp.__init__(self)
		self.__left = left
		self.__right = right

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, UnionRegExp) and \
				self.__left == other.getLeftRegExp() and \
				self.__right == other.getRightRegExp()

	def __unicode__(self):
		"""Overrides base class method."""
		return unicode(self.__left) + "+" + unicode(self.__right)

	def getLeftRegExp(self):
		"""Returns stored left regexp."""
		return self.__left

	def getRightRegExp(self):
		"""Returns stored right regexp."""
		return self.__right

class ConcatRegExp(RegExp):
	"""Represents concatenation of two regexps"""

	def __init__(self, left, right):
		"""Constructor.

		left - First regexp
		right - Second regexp
		"""
		assert isinstance(left, RegExp), left.__class__
		assert isinstance(right, RegExp), right.__class__
		RegExp.__init__(self)
		self.__left = left
		self.__right = right

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, ConcatRegExp) and \
				self.__left == other.getLeftRegExp() and \
				self.__right == other.getRightRegExp()

	def __unicode__(self):
		"""Overrides base class method."""
		return unicode(self.__left) + unicode(self.__right)

	def getLeftRegExp(self):
		"""Returns stored left regexp."""
		return self.__left

	def getRightRegExp(self):
		"""Returns stored right regexp."""
		return self.__right

class IterRegExp(RegExp):
	"""Represents iteration on a regexp"""

	def __init__(self, r):
		"""Constructor.

		r - Regexp to be iterated.
		"""
		RegExp.__init__(self)
		self.__r = r

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, IterRegExp) and \
				self.__r == other.getRegExp()

	def __unicode__(self):
		"""Overrides base class method."""
		return unicode(self.__r) + "*"

	def getRegExp(self):
		"""Returns stored regexp."""
		return self.__r

class BracketsRegExp(RegExp):
	"""Represents regexp in brackets"""

	def __init__(self, r):
		"""Constructor.

		r - Regexp that is in brackets.
		"""
		RegExp.__init__(self)
		self.__r = r

	def __eq__(self, other):
		"""Overrides base class method."""
		return isinstance(other, BracketsRegExp) and \
				self.__r == other.getRegExp()

	def __unicode__(self):
		"""Overrides base class method."""
		return "(" + unicode(self.__r) + ")"

	def getRegExp(self):
		"""Returns stored regexp."""
		return self.__r

