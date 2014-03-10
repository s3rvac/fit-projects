# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

"""This module provides creation of images from textual representation
of NFAs in the DOT language.

Requirements:
  - functions in this module requires graphviz (http://www.graphviz.org)
    to be installed and the 'dot' program to be in PATH (so it can
    be ran via `dot`)
"""


import os
import subprocess
import tempfile


# Minimal font size (dot program minimum), in points
MIN_FONT_SIZE = 1.0

# Default font size, in points
DEF_FONT_SIZE = 14.0

# Path to the dot program
_DOT_PROGRAM_PATH = 'dot'


class ImageFormats(object):
	"""Image formats, which can be passed to the createImage() function."""
	JPG = 'jpg'
	JPEG = 'jpeg'
	PNG = 'png'


class DotProgramError(Exception):
	"""The dot program returned some non-zero return code."""
	def __init__(self, msg):
		self.msg = msg

	def __str__(self):
		return repr(self.msg)


def createImage(nfaRepr, imgFormat, imgLoc, fontSize=DEF_FONT_SIZE):
	"""Creates an image from the selected NFA representation in the selected
	format, stores it in the selected location and returns the name of the image
	file (string).

	nfaRepr - NFA representation in the DOT language (unicode string)
	imgFormat - image format (ImageFormats.X)
	imgLoc - path to a directory into which the resulting image will be stored
	fontSize - size of used text font in points (float number)

	Returned file path will start with the given path (imgLoc) and ends with
	the image format extension (ImageFormats.X).

	Raises:
	  - IOError if there is some I/O error
	  - OSError if there is some OS error
	  - DotProgramError if the dot program returned some non-zero return code
	    or it is not installed

	Preconditions:
	  - imgFormat must be a constant from the ImageFormats class
	  - fontSize >= MIN_FONT_SIZE
	"""
	assert fontSize >= MIN_FONT_SIZE, fontSize

	# Create the output image file (to make sure that the file name will be unique)
	(imgFile, imgFilePath) = tempfile.mkstemp(suffix='.' + imgFormat, dir=imgLoc)
	os.close(imgFile)

	# Create program arguments
	args = ''
	for argName, argVal in [
			# Image format
			('T', imgFormat),
			# Path to the output image file
			('o', imgFilePath),
			# Draw the graph in a left-to-right manner
			('G', 'rankdir=LR'),
			# Text font size
			('N', 'fontsize=' + str(fontSize)),
			('E', 'fontsize=' + str(fontSize))
			]:
		args += ' -' + argName + argVal

	# If the system is GNU/Linux, redirect stderr to /dev/null, so error messages
	# won't be printed on the console
	try:
		stderrRedir = ' 2> /dev/null' if os.uname()[0] == 'Linux' else ''
	except AttributeError:
		# There is no os.uname() on MS Windows, so that's why this fallback
		stderrRedir = ''

	# Create the image by calling the dot program
	dotProc = subprocess.Popen(_DOT_PROGRAM_PATH + args + stderrRedir,
			shell=True, stdin=subprocess.PIPE)
	# The dot program expects data on stdin, so send them and wait
	# until it terminates.
	# We also have to encode the data before they are sent to the processes stdin.
	dotProc.communicate(nfaRepr.encode('utf8'))
	if dotProc.returncode != 0:
		# Cleanup
		os.remove(imgFilePath)
		raise DotProgramError, str(dotProc.returncode)

	return imgFilePath
