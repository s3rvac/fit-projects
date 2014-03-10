#!/usr/bin/env python
# -*- coding: utf8 -*-
#
# Project: ISJ - HTML lists of publications to BibTeXML converter
# Author:  Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 9.2.2008
#

import os
import sys

from pubs2BibTeXML import *

TEST_OUTPUTS = os.path.join('test-outputs', 'outputs')
TEST_REFERENCE_OUTPUTS = os.path.join(TEST_OUTPUTS, 'reference')

for site, refCount in [
	('http://www.fit.vutbr.cz/~zendulka/pubs.php.en', 11),
	('http://tony.abou-assaleh.net/Publications', 30),
	('http://www-personal.umich.edu/~ladamic/', 22),
	('http://www.cond.org/publications.html', 53),
	('http://www-scf.usc.edu/~alcazar/#_List_of_publications', 23),
	('http://ixa.si.ehu.es/Ixa/Argitalpenak/kidearen_argitalpenak?kidea=1000808989', 89),
	('http://www.dfki.de/~janal/publications.html', 54),
	('http://www.cs.rochester.edu/u/james/', 24),
	('http://www.dc.fi.udc.es/~alonso/papers.html', 108),
	('http://www.bme.ogi.edu/~hynek/cgi-bin/publications/showbib_asp.pl?all', 83)]:
	try:
		publications = []
		inputData = getRawInputData(site)
		publications = merge(publications, parseInputDataIntoPublications(
			inputData, site))

		# Publications count
		#print site, len(publications), refCount
		if len(publications) != refCount:
			print site, len(publications), refCount

		# BibTeXML
		f = open(os.path.join(TEST_OUTPUTS, site.replace('/', '_')), 'w')
		f.write(publications2BibTeXML(publications, False))
		f.close()
	except Exception, e:
		print site, str(e)
