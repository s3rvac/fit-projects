# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Converts regexp trees to finite automata."""

from regexp import *
from nfas import *

def tree2nfa(tree):
	"""Converts regexp trees to finite automata.

	tree - RegExp object

	Returns:
	[(NFA, re)] is list of pairs where NFA is an instance of NFA class
	that represents the RegExp re. The list is created as postorder pass
	through the given tree. First pair represent the leftmost leaf and last
	pair represents the given root (tree). So statement
	'tree == tree2nfa(tree)[-1][1]' is always true.
	"""
	assert isinstance(tree, RegExp), tree.__class__
	# Leafs
	if isinstance(tree, EmptySetRegExp):
		return [(EmptySetNFA(), tree)]

	elif isinstance(tree, EpsilonRegExp):
		return [(SingleSymbolNFA(""), tree)]

	elif isinstance(tree, SingleSymbolRegExp):
		return [(SingleSymbolNFA(tree.getSymbol()), tree)]

	elif isinstance(tree, UnionRegExp):
		subL = tree2nfa(tree.getLeftRegExp())
		subR = tree2nfa(tree.getRightRegExp())
		sub = subL + subR
		sub.append((UnionNFA(subL[-1][0], subR[-1][0]), tree))
		return sub

	elif isinstance(tree, ConcatRegExp):
		subL = tree2nfa(tree.getLeftRegExp())
		subR = tree2nfa(tree.getRightRegExp())
		sub = subL + subR
		sub.append((ConcatNFA(subL[-1][0], subR[-1][0]), tree))
		return sub

	elif isinstance(tree, IterRegExp):
		sub = tree2nfa(tree.getRegExp())
		sub.append((IterNFA(sub[-1][0]), tree))
		return sub

	elif isinstance(tree, BracketsRegExp):
		sub = tree2nfa(tree.getRegExp())
		sub.append((sub[-1][0], tree))
		return sub

	else:
		assert False, "Impossible has just happened."
