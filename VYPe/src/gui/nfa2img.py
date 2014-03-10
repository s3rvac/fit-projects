# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#	  Libor Polčák, xpolca03@stud.fit.vutbr.cz
#	  Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""This module provides an easy-to-use way how to convert
NFA instances directly into a wx representation of images.
"""


import os
import tempfile
import wx

from ..lazy_map import lazyMap
from ..dot import NumberGenerator, renameStates
from ..dot2img import createImage, ImageFormats, DotProgramError
from ..nfa2dot import convertNFAToDOT, DEF_HIGHLIGHT_COLOR


class CreateNFAImageError(Exception):
	"""There was some error when creating an image for an NFA."""
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return repr(self.msg)


def createNFAImages(nfas, fontSize, highlightColor=DEF_HIGHLIGHT_COLOR):
	"""Creates and returns a "list" of images (wx.Image instances) in the
	PNG format for the selected "list" of NFAs. "List" is in double quotes,
	because it does not return a list, but a list resembling object. Each
	image is created only if it is accessed and that it is cached. Note
	that CreateNFAImageError can be thrown when accessing elements from the
	returned list (it means that the NFA on that index could not be
	converted into an image).

	nfas - list of NFA instances
	fontSize - size of the font to be used in created images
	highlightColor - color to be used for highlighted states and
	                 transitions (string)
	"""
	class ConvertNFAIntoImage(object):
		"""Function object for creating nfa images from NFA instances."""
		def __init__(self, fontSize):
			"""Creates a new function object.

			fontSize - size of the font to be used in created images
			"""
			self.__statesMap = {}
			self.__nextNameFunc = NumberGenerator()
			self.__fontSize = fontSize

		def __call__(self, nfa):
			"""Converts the selected NFA instance into a PNG image
			and returns a wx.Image instance.

			Raises CreateNFAImageError if there is a conversion error."""
			try:
				# Convert the NFA into an image file
				dotRepr = convertNFAToDOT(nfa, highlightColor)
				dotRepr = renameStates(dotRepr, self.__statesMap,
						self.__nextNameFunc)
				nfaImgPath = createImage(dotRepr, ImageFormats.PNG,
						tempfile.gettempdir(), self.__fontSize)

				# Load that image file into the memory and store it
				nfaImg = wx.Image(nfaImgPath, wx.BITMAP_TYPE_PNG)

				# Since the image is in the memory, we can removed that image file
				os.remove(nfaImgPath)

				return nfaImg
			except IOError, e:
				raise CreateNFAImageError, e.message
			except OSError, e:
				raise CreateNFAImageError, e.message
			except DotProgramError, e:
				raise CreateNFAImageError, e.message

	# Use lazyMap to improve performance
	return lazyMap(ConvertNFAIntoImage(fontSize), nfas)
