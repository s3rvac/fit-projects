# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Regular expression parsing."""

from regexp import *

class RegExpParser(object):
	"""Library of methods to parse regexps."""

	@staticmethod
	def parseRegExp(regExp):
		"""Creates tree representing given regexp."""
		assert isinstance(regExp, basestring), regExp.__class__
		tree, regExp = RegExpParser.__S(regExp)
		if len(regExp) > 0:
			raise ValueError("Trailing " + regExp + " at the end of given expression")
		return tree

	@staticmethod
	def __S(regExp):
		"""Tries to create subtree from nonterminal S and given string.

		Union may be created
		"""
		# S -> U S'
		left, regExp = RegExpParser.__U(regExp)
		right, regExp = RegExpParser.__S_(regExp)
		united = left
		for r in right:
			united = UnionRegExp(united, r)
		return (united, regExp)

	@staticmethod
	def __S_(regExp):
		"""Tries to create subtree from nonterminal S' and given string.

		Consumes right part of union or empty string
		"""
		if len(regExp) > 0 and regExp[0] == "+":
			# S' -> + U S'
			left, regExp = RegExpParser.__U(regExp[1:])
			right, regExp = RegExpParser.__S_(regExp)
			return ([left] + right, regExp)
		else:
			# S' -> eps
			return ([], regExp)

	@staticmethod
	def __U(regExp):
		"""Tries to create subtree from nonterminal U and given string.

		Concatenation may be created
		"""
		# U -> K U'
		left, regExp = RegExpParser.__K(regExp)
		right, regExp = RegExpParser.__U_(regExp)
		concatenated = left
		for r in right:
			concatenated = ConcatRegExp(concatenated, r)
		return (concatenated, regExp)

	@staticmethod
	def __U_(regExp):
		"""Tries to create subtree from nonterminal U' and given string.

		Consumes right part of concatenation or empty string
		"""
		if len(regExp) > 0 and regExp[0] not in ["+", ")", "*"]:
			# U' -> K U'
			left, regExp = RegExpParser.__K(regExp)
			right, regExp = RegExpParser.__U_(regExp)
			return ([left] + right, regExp)
		else:
			# U' -> eps
			return ([], regExp)

	@staticmethod
	def __K(regExp):
		"""Tries to create subtree from nonterminal K and given string.

		Iteration may be created
		"""
		tree, regExp = RegExpParser.__I(regExp)
		return RegExpParser.__K_(tree, regExp)

	@staticmethod
	def __K_(tree, regExp):
		"""Tries to create subtree from nonterminal K' and given string.

		Consumes iteration or empty string
		"""
		if len(regExp) > 0 and regExp[0] == "*":
			# K' -> * K'
			return RegExpParser.__K_(IterRegExp(tree), regExp[1:])
		else:
			# K' -> eps
			return (tree, regExp)

	@staticmethod
	def __I(regExp):
		"""Tries to create subtree from nonterminal I and given string.

		One symbol or regexp in bracket can be represented
		"""
		assert isinstance(regExp, basestring), regExp.__class__
		if len(regExp) == 0:
			raise ValueError("Given regular expression is not finished correctly, symbol expected")
		first = regExp[0]
		if first == "(":
			# I -> ( S )
			tree, regExp = RegExpParser.__S(regExp[1:])
			if regExp == "":
				raise ValueError("Given regular expression is not finished correctly," +\
						" ) was expected but nothing was found")
			if regExp[0] != ")":
				raise ValueError(") was expected but " + regExp + " was found")
			return (BracketsRegExp(tree), regExp[1:])
		elif first == "\\":
			# Escaped symbol
			if len(regExp) == 1:
				raise ValueError("escape sequence was not finished correctly")
			escaped = regExp[1]
			regExp = regExp[2:]
			if escaped == "e":
				# I -> \e
				return (EpsilonRegExp(), regExp)
			elif escaped == "0":
				# I -> \0
				return (EmptySetRegExp(), regExp)
			elif escaped == "\\":
				# I -> \\
				return (SingleSymbolRegExp(escaped), regExp)
			elif escaped == "(":
				# I -> \(
				return (SingleSymbolRegExp(escaped), regExp)
			elif escaped == ")":
				# I -> \)
				return (SingleSymbolRegExp(escaped), regExp)
			elif escaped == "+":
				# I -> \+
				return (SingleSymbolRegExp(escaped), regExp)
			elif escaped == "*":
				# I -> \*
				return (SingleSymbolRegExp(escaped), regExp)
			else:
				raise ValueError("Unknown escape sequence \\" + escaped + " used")
		elif first in ["*", "+", ")"]:
			# undefined derivation
			raise ValueError("Unexpected " + first + " found")
		else:
			# I -> alphabet Symbol
			return (SingleSymbolRegExp(first), regExp[1:])

