# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

import os
import re
import unittest
import sys
import tempfile

from .. import dot2img


class CreateImageTests(unittest.TestCase):
	def setUp(self):
		pass

	def tearDown(self):
		pass

	def scenarioTestValidRepresentationNoError(self, nfaRepr, imgFormat, imgLoc,
			fontSize=dot2img.DEF_FONT_SIZE):
		try:
			imgPath = dot2img.createImage(nfaRepr, imgFormat, imgLoc, fontSize)
			# File was created
			self.assertTrue(os.path.isfile(imgPath))
			# File is non-empty
			self.assertTrue(os.path.getsize(imgPath) > 0)
			# File path is in proper format
			self.assertTrue(imgPath.startswith(imgLoc))
			self.assertTrue(imgPath.endswith('.' + imgFormat))
			# Cleanup
			os.remove(imgPath)
		except IOError, e:
			self.fail("IOError was raised with the message: " + str(e) +\
				". Do you have the graphviz package installed?")
		except OSError, e:
			self.fail("OSError was raised with the message: " + str(e) +\
				". Do you have the graphviz package installed?")
		except dot2img.DotProgramError, e:
			self.fail("The dot program returned: " + str(e) +\
				". Do you have the graphviz package installed?")

	def testValidRepresentationWriteableLocationDefaultFontSize(self):
		nfaRepr = u"digraph finite_state_machine {\n" +\
			u"node [shape = point]; SA;\n" +\
			u"node [shape = circle]; A;\n" +\
			u'SA -> A [ label = "" ];\n' +\
			u"}\n"
		imgFormat = dot2img.ImageFormats.PNG
		imgLoc = tempfile.gettempdir()
		self.scenarioTestValidRepresentationNoError(nfaRepr, imgFormat, imgLoc)

	def testValidRepresentationWriteableLocationCustomFontSize(self):
		nfaRepr = u"digraph finite_state_machine {\n" +\
			u"node [shape = point]; SA;\n" +\
			u"node [shape = circle]; A;\n" +\
			u'SA -> A [ label = "" ];\n' +\
			u"}\n"
		imgFormat = dot2img.ImageFormats.PNG
		imgLoc = tempfile.gettempdir()
		fontSize = 32.0
		self.scenarioTestValidRepresentationNoError(nfaRepr, imgFormat, imgLoc,
				fontSize)

	def testValidRepresentationNonWriteableLocation(self):
		nfaRepr = u"digraph finite_state_machine {\n" +\
			u"node [shape = point]; SA;\n" +\
			u"node [shape = circle]; A;\n" +\
			u'SA -> A [ label = "" ];\n' +\
			u"}\n"
		imgFormat = dot2img.ImageFormats.PNG
		imgLoc = '$%^$%&^&*^*' # Some garbage
		try:
			dot2img.createImage(nfaRepr, imgFormat, imgLoc)
		except IOError:
			pass # OK
		except OSError:
			pass # OK
		else:
			self.fail("Exception should have been thrown.")

	def testInvalidRepresentationWriteableLocation(self):
		nfaRepr = u"digraph finite_state_machine {"
		imgFormat = dot2img.ImageFormats.PNG
		imgLoc = tempfile.gettempdir()
		try:
			dot2img.createImage(nfaRepr, imgFormat, imgLoc)
		except dot2img.DotProgramError:
			pass # OK
		else:
			self.fail("Exception should have been thrown.")


def suite():
	"""Returns a test suite that contains all tests from this module."""
	return unittest.TestLoader().loadTestsFromModule(sys.modules[__name__])


def test():
	"""Runs all unit tests for this module."""
	runner = unittest.TextTestRunner()
	runner.run(suite())


if __name__ == '__main__':
	test()
