# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""This module provides a basic lazy map (map, that returns a list, but items
of that list are computed only when they're accessed and then they're cached)
implementation.
"""

class _LazyMap(object):
	"""Object returned by the lazyMap() function, which handles the
	laziness.
	"""

	def __init__(self, func, seq):
		"""Creates an empty list-like object. See lazyMap() function
		documentation for more info about the parameters and behaviour.
		"""
		self.__func = func
		self.__seq = seq
		self.__computed = {}

	def __len__(self):
		"""Returns the number of accessed (computed) elements."""
		return len(self.__computed.keys())

	def __getitem__(self, i):
		"""Returns (and computes, if necessary) the ith element.

		Raises IndexError if the selected index is out of bounds.
		Currently, i must be >= 0."""
		# Check bounds
		if i >= len(self.__seq) or i < 0:
			raise IndexError, 'list index out of range'

		# Check if this element has been computed and if not, compute it
		# and cache it
		if not self.__computed.has_key(str(i)):
			self.__computed[str(i)] = self.__func(self.__seq[i])

		# Return cached result
		return self.__computed[str(i)]


def lazyMap(func, seq):
	"""Applies the func function to all elements from seq and returns a
	list-like object, which is initially empty. When you access some of its
	element, it value will be computed and stored for the next access. If you
	don't access an element, its value will never be computed.

	func - unary function
	seq - sequence of elements

	The returned object does not raises any exceptions except IndexError
	and if the func raises some exception, it is propagated to the calling code.

	Limitations:
	  - only the following basic operations over the returned list-like object
	    are implemented (x is the object returned by this function, n is a number):
	        * len(x)
	        * x[n], n >= 0
	"""
	return _LazyMap(func, seq)
