# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""Transformation of NFA objects (instances of the NFA class) into their
textual representation in the DOT language, which is a language for
describing diagrams used by a program named Graphviz, which can
create nice images from this representation.
"""

import nfas


# String representation of the epsilon transition symbol
# in the textual representation (see convertNFAToDOT() description)
EMPTY_STRING_TRANS_SYM = u'ε'

# Default highlighting color
DEF_HIGHLIGHT_COLOR = u'red'
DEF_NORMAL_COLOR = u'black'


def convertNFAToDOT(nfa, highlightColor=DEF_HIGHLIGHT_COLOR,
		normalColor=DEF_NORMAL_COLOR):
	"""Converts the selected NFA into a textual representation in the DOT
	language (see http://www.graphviz.org/doc/info/lang.html for more
	details on the format).

	nfa - NFA (instance of the NFA class)
	highlightColor - color to be used for highlighted states and
	                 transitions (string)
	normalColor - color to be used for non-highlighted states and
	                 transitions (string)

	Returns a unicode string in the following format:
		"digraph finite_state_machine {\n"
		NODES
		TRANSITIONS
		"}\n"
	where:

	Let COLOR be the appropriate color of a state or transition
	(it depends on whether the selected state or transition will
	be highlighted).

	NODES is
		'node [shape = doublecircle, color = COLOR, fontcolor = COLOR ]; X;\n'
	for a final state X,
		'node [shape = point, color = COLOR, fontcolor = COLOR]; SX;\n'
		'node [shape = circle, color = COLOR]; X;\n'
	for a starting state X,
		'node [shape = circle, color = COLOR]; X;\n'
	for each other state X. Other states are presented after
	the starting and the final state (if any).

	TRANSITIONS is
		'SX -> X [ label = "", color = COLOR, fontcolor = COLOR ];\n'
	for a starting state X,
		'X -> Y [ label = "C", color = COLOR, fontcolor = COLOR ];\n'
	for each transition from X to Y with symbol C (C != '') and
		'X -> Y [ label = "' + EMPTY_STRING_TRANS_SYM + '",
			fontname = "Times-Italic", color = COLOR, fontcolor = COLOR ];\n'
	for each epsilon tansition from X to Y.
	"""
	# Returns color of the selected state
	def getStateColor(state, highlightColor, normalColor):
		# Shape color
		color = ', color = '
		if state.wasChanged():
			color += highlightColor
		else:
			color += normalColor
		# State name color
		color += color.replace(', color = ', ', fontcolor = ')
		return color

	# Returns color of the selected transition
	def getTransColor(fromState, toState, highlightColor, normalColor):
		# Edge color ('color')
		color = ', color = '
		if fromState.wasChanged() and toState.wasChanged():
			if fromState.isStarting() or toState.isFinal():
				color += highlightColor
			# HACK: To solve redundant highlighting in a regular expression
			# like a*, where a is an arbitrary symbol, this checking
			# has to be done. Yes, shame on me, but this is the only
			# downside of my elegant highlighting solution...
			elif isinstance(nfa, nfas.IterNFA) and fromState.getName() <\
					toState.getName():
				color += normalColor
			else:
				color += highlightColor
		else:
			color += normalColor
		# Transitions symbol color ('fontcolor')
		color += color.replace(', color = ', ', fontcolor = ')
		return color

	# Prologue
	res = u'digraph finite_state_machine {\n'

	# Add the starting state
	ss = nfa.getStartingState()
	ssColor = getStateColor(ss, highlightColor, normalColor)
	res += u'node [shape = point' + ssColor + ']; S' + ss.getName() + u';\n'
	res += u'node [shape = circle' + ssColor + ']; ' + ss.getName() + u';\n'

	# Add the final state (if any)
	if len(nfa.getFinalStates()) == 1:
		fs = nfa.getFinalStates()[0]
		res += u'node [shape = doublecircle' + getStateColor(fs, highlightColor,
			normalColor) + ']; ' + fs.getName() + u';\n'

	# Add other states
	for state in nfa.getStates():
		if not state.isStarting() and not state.isFinal():
			res += u'node [shape = circle' + getStateColor(state, highlightColor,
					normalColor) + ']; ' + state.getName() + u';\n'

	# Add transitions
	# Starting state transition
	res += u'S' + ss.getName() + u' -> ' + ss.getName() + u' [ label = ""' +\
			getStateColor(ss, highlightColor, normalColor) + ' ];\n'
	# Other transitions
	for (fromState, symbol, toState) in nfa.getTransitions():
		# Label
		label = u' label = "'
		if symbol == u'':
			# Turn the empty string symbol into a visible representation
			# and show it in italic
			label += EMPTY_STRING_TRANS_SYM + u'", fontname = "Times-Italic"'
		elif symbol in [u'"', u'\\']:
			# Quote or backslash as a symbol must be prefixed,
			# because the symbol appears between two quotes
			label += u'\\' + symbol + u'"'
		else:
			label += symbol + u'"'

		res += fromState.getName() + u' -> ' + toState.getName() + ' [' + label +\
				getTransColor(fromState, toState, highlightColor, normalColor) + u' ];\n'

	# Epilogue
	res += u'}\n'

	return res
