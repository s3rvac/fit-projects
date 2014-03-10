# vim:fileencoding=utf8
#
# Project: VYPe - Conversion from regular expressions to finite automata
# Authors:
#     Libor Polčák, xpolca03@stud.fit.vutbr.cz
#     Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 4.10.2009
#

import wx

def displayErrorMessage(parent, text):
	"""Displays error message with the selected text."""
	dialog = wx.MessageDialog(parent, text, 'Error', wx.OK | wx.ICON_ERROR)
	dialog.CenterOnScreen()
	dialog.ShowModal()
	dialog.Destroy()

def displayWarningMessage(parent, text):
	"""Displays warning message with the selected text."""
	dialog = wx.MessageDialog(parent, text, 'Warning', wx.OK | wx.ICON_WARNING)
	dialog.CenterOnScreen()
	dialog.ShowModal()
	dialog.Destroy()

def displayInformationMessage(parent, text):
	"""Displays information message with the selected text."""
	dialog = wx.MessageDialog(parent, text, 'Information', wx.OK | wx.ICON_INFORMATION)
	dialog.CenterOnScreen()
	dialog.ShowModal()
	dialog.Destroy()
