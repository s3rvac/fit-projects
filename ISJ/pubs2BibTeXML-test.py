#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Project: ISJ - HTML pages with publications to BibTeXML convertor
# Author:  Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 9.2.2008
#

"""Testing suite for pubs2BibTeXML.py script."""

import codecs
import os
import shutil
import sys
import tempfile
from unittest import TestCase, TextTestRunner, TestSuite, makeSuite
from pubs2BibTeXML import *

class IOFunctionsTests(TestCase):
	def setUp(self):
		self.tempDirectoryPath = tempfile.mkdtemp()

	def tearDown(self):
		shutil.rmtree(self.tempDirectoryPath, True)

############################## openSourceStream ################################
	def testOpenSourceStreamForEmptyStringReturnsSysStdin(self):
		self.assertEqual(openSourceStream(''), sys.stdin)

	def testOpenSourceStreamInvalidSourceRaisesIOError(self):
		try:
			openSourceStream('#')
		except IOError:
			pass
		else:
			self.fail('IOError should have been thrown.')

################################ getRawInputData ###############################
	def testGetRawInputDataFromUtf8FileReturnsUtf8EncodedString(self):
		inputFilePath = os.path.join(self.tempDirectoryPath, 'utf8.txt')
		fileContent = u'éíáýžřčšě+sdfsdf@#@$#^$%'
		codecs.open(inputFilePath, 'w', 'utf8').write(fileContent)

		self.assertEqual(getRawInputData(inputFilePath), fileContent)

	def testGetRawInputDataFromLatin2FileReturnsUtf8EncodedString(self):
		inputFilePath = os.path.join(self.tempDirectoryPath, 'latin2.txt')
		fileContent = u'éíáýžřčšě+sdfsdf@#@$#^$%'
		codecs.open(inputFilePath, 'w', 'latin2').write(
			fileContent.encode('utf8').decode('latin2'))

		self.assertEqual(getRawInputData(inputFilePath), fileContent)

class Pubs2BibTeXMLTests(TestCase):
############################# createPublicationUrl #############################
	def testCreatePublicationUrlUrlFromPublicationAbsolutePath(self):
		sourceName = 'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en'
		urlFromPublication = '/research/view_pub.php.en?id=8058'
		expectedUrl = 'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8058'
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

	def testCreatePublicationUrlUrlFromPublicationRelativePath(self):
		sourceName = 'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en'
		urlFromPublication = 'view_pub.php.en?id=8058'
		expectedUrl = 'http://www.fit.vutbr.cz/%7Ezendulka/view_pub.php.en?id=8058'
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

	def testCreatePublicationUrlUrlFromPublicationRelativePathWithDotSlash(self):
		sourceName = 'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en'
		urlFromPublication = './view_pub.php.en?id=8058'
		expectedUrl = 'http://www.fit.vutbr.cz/%7Ezendulka/view_pub.php.en?id=8058'
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

	def testCreatePublicationUrlUrlFromPublicationCompleteUrl(self):
		sourceName = 'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en'
		urlFromPublication = 'http://muni.cz/pubs/view_pub.php.en?id=8058'
		expectedUrl = 'http://muni.cz/pubs/view_pub.php.en?id=8058'
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

	def testCreatePublicationUrlUrlFromPublicationQueryPartInSourceName(self):
		sourceName = 'http://www.bme.ogi.edu/~hynek/cgi-bin/publications/showbib_asp.pl?all'
		urlFromPublication = 'http://www.bme.ogi.edu/~hynek/publications/pdf/takajuki.pdf'
		expectedUrl = 'http://www.bme.ogi.edu/~hynek/publications/pdf/takajuki.pdf'
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

	def testCreatePublicationUrlUrlFromPublicationEmptyUrls(self):
		sourceName = ''
		urlFromPublication = ''
		expectedUrl = ''
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

	def testCreatePublicationUrlUrlFromPublicationNonsenseData(self):
		sourceName = '9#$$/6 567/%&%*^*'
		urlFromPublication = '@#$%$ 3$% #^ $^$&%^&/ 5/6 5/6 7/567 '
		expectedUrl = urlFromPublication
		self.assertEqual(expectedUrl, createPublicationUrl(sourceName, urlFromPublication))

################################## splitLine ###################################
	def scenarioTestSplitLine(self, line, expectedResult):
		self.assertEqual(expectedResult, splitLine(line))

	def testSplitLineNoNestedTags(self):
		self.scenarioTestSplitLine("""<b>Rudolfová Ivana, Zendulka Jaroslav</b>: <a href="/research/view_pub.php.en?id=8058">Clustering of Protein Sequences</a>, In: Proceedings of 1st International Workshop WFM'06, Přerov, CZ, MARQ, 2006, <nobr>p.&nbsp;71-78</nobr>, <nobr>ISBN 80-86840-20-4</nobr>""",
			[{'tag': 'b', 'text': 'Rudolfová Ivana, Zendulka Jaroslav'},
			{'tag': 'a', 'attributes': 'href="/research/view_pub.php.en?id=8058"', 'text': 'Clustering of Protein Sequences'},
			{'text': 'In'},
			{'text': "Proceedings of 1st International Workshop WFM'06"},
			{'text': 'Přerov'},
			{'text': 'CZ'},
			{'text': 'MARQ'},
			{'text': '2006'},
			{'tag': 'nobr', 'text': 'p.&nbsp;71-78'},
			{'tag': 'nobr', 'text': 'ISBN 80-86840-20-4'}])

############################### isValidPublication #############################
	def testIsValidPublicationNoKeys(self):
		self.assertEqual(False, isValidPublication({}))

	def testIsValidPublicationOnlyAuthor(self):
		self.assertEqual(False, isValidPublication(
			{'author': 'Petr Zemek'}))

	def testIsValidPublicationOnlyTitle(self):
		self.assertEqual(False, isValidPublication(
			{'title': 'Dreaming and sleeping'}))

	def testIsValidPublicationAuthorAndTitle(self):
		self.assertEqual(True, isValidPublication(
			{'author': 'Petr Zemek', 'title': 'Dreaming and sleeping'}))

########################### parseRawLineIntoPublication ########################
	def scenarioTestParseRawLine(self, line, expectedResult, sourceName = '', usableWords = []):
		self.assertEqual(expectedResult, parseRawLineIntoPublication(line, sourceName, usableWords))

	def testParseRawLineZendulka1(self):
		# Missing: MARQ
		self.scenarioTestParseRawLine(u"""<b>Rudolfová Ivana, Zendulka Jaroslav</b>: <a href="/research/view_pub.php?id=8058">Clustering of Protein Sequences</a>, In: Proceedings of 1st International Workshop WFM'06, Přerov, CZ, MARQ, 2006, <nobr>p.&nbsp;71-78</nobr>, <nobr>ISBN 80-86840-20-4</nobr>""",
			{'pubType': u'inproceedings',
			 'author': u'Rudolfová Ivana and Zendulka Jaroslav',
			 'title': u'Clustering of Protein Sequences',
			 'pages': u'71-78',
			 'booktitle': u'Proceedings of 1st International Workshop WFM\'06',
			 'year': u'2006',
			 'location': u'Přerov, CZ',
			 'publisher': u'MARQ',
			 'isbn': u'80-86840-20-4',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php?id=8058'},
			'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en',
			['Jaroslav', 'Zendulka'])

	def testParseRawLineZendulka2(self):
		self.scenarioTestParseRawLine(u"""<b>Zendulka Jaroslav (editor)</b>: <a href="/research/view_pub.php?id=8255">Proceedings of 9th International Conference ISIM'06</a>, Ostrava, CZ, MARQ, 2006, <nobr>p.&nbsp;192</nobr>, <nobr>ISBN 80-86840-19-0</nobr>""",
			{'pubType': u'proceedings',
			 'editor': u'Zendulka Jaroslav',
			 'title': u"Proceedings of 9th International Conference ISIM'06",
			 'pages': u'192',
			 'year': u'2006',
			 'location': u'Ostrava, CZ',
			 'publisher': u'MARQ',
			 'isbn': u'80-86840-19-0',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php?id=8255'},
			'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en',
			['Jaroslav', 'Zendulka'])

	def testParseRawLineSmrz1(self):
		self.scenarioTestParseRawLine(u"""<b>Fapšo Michal, Smrž Pavel, Schwarz Petr, Szöke Igor, Schwarz Milan, Černocký Jan, Karafiát Martin, Burget Lukáš</b>: <a href="/research/view_pub.php?id=7922">Information Retrieval from Spoken Documents</a>, In: Proceedings of the Seventh International Conference on Intelligent Text Processing and Computational Linguistics (CICLING 2006), Mexico City, MX, Springer, 2006, <nobr>s.&nbsp;410-416</nobr>, <nobr>ISBN 3-540-32205-1</nobr>""",
			{'pubType': u'inproceedings',
			 'author': u'Fapšo Michal and Smrž Pavel and Schwarz Petr and Szöke Igor and Schwarz Milan and Černocký Jan and Karafiát Martin and Burget Lukáš',
			 'title': u'Information Retrieval from Spoken Documents',
			 'pages': u'410-416',
			 'booktitle': u'Proceedings of the Seventh International Conference on Intelligent Text Processing and Computational Linguistics (CICLING 2006)',
			 'location': u'Mexico City, MX',
			 'publisher': u'Springer',
			 'year': u'2006',
			 'isbn': u'3-540-32205-1',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php?id=7922'},
			'http://www.fit.vutbr.cz/~smrz/pubs.php',
			['Pavel', u'Smrž'])

	def testParseRawLineMeduna1(self):
		self.scenarioTestParseRawLine(u"""<b>Meduna Alexander, Techet Jiří</b>: <a href="/research/view_pub.php.en?id=8377">Canonical Scattered Context Generators of Sentences with Their Parses</a>, In: Theoretical Computer Science, Vol.&nbsp;2007, No.&nbsp;389, Paris, FR, <nobr>p.&nbsp;73-81</nobr>, <nobr>ISSN 0304-3975</nobr>""",
			{'pubType': u'article',
			 'author': u'Meduna Alexander and Techet Jiří',
			 'title': u'Canonical Scattered Context Generators of Sentences with Their Parses',
			 'pages': u'73-81',
			 'journal': u'Theoretical Computer Science',
			 'volume': u'2007',
			 'number': u'389',
			 'location': u'Paris, FR',
			 'year': u'2007',
			 'issn': u'0304-3975',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8377'},
			'http://www.fit.vutbr.cz/~meduna/pubs.php.en',
			['Alexander', 'Meduna'])

	def testParseRawLineAbouAssaleh1(self):
		# Missing: COMPSAC 2004
		# TODO - "Ke&#353;elj, V" in the input source makes problem
		self.scenarioTestParseRawLine(u"""Abou-Assaleh, T., Cercone, N., Kešelj, V., &amp; Sweidan, R. (2004). <em>N-gram-based Detection of New Malicious Code</em>. In Proceedings of the 28th IEEE Annual International Computer Software and Applications Conference, COMPSAC 2004, Hong Kong.""",
			{'pubType': u'inproceedings',
			 'author': u'T. Abou-Assaleh and N. Cercone and V. Kešelj and R. Sweidan',
 			 'title': u'N-gram-based Detection of New Malicious Code',
			 'booktitle': u'Proceedings of the 28th IEEE Annual International Computer Software and Applications Conference', #, COMPSAC 2004',
			 'year': u'2004',
			 'address': u'Hong Kong'},
			 'http://tony.abou-assaleh.net/publications',
			['Tony', 'Abou-Assaleh'])

	def testParseRawLineAbouAssaleh2(self):
		# TODO - "Relaxed Unification &#8212; Proposal." in the input source makes problem
		self.scenarioTestParseRawLine(u"""Abou-Assaleh, T., &amp; Cercone, N. (2003). Relaxed Unification - Proposal. <em>Applied Mathematics Letters</em>, 16(5), 747-751.""",
			{'pubType': u'article',
			 'author': u'T. Abou-Assaleh and N. Cercone',
 			 'title': u'Relaxed Unification - Proposal',
			 'journal': u'Applied Mathematics Letters',
			 'year': u'2003',
			 'volume': u'16',
			 'pages': u'747-751',
			 'number': u'5'},
			 'http://tony.abou-assaleh.net/publications',
			['Tony', 'Abou-Assaleh'])

	def testParseRawLineAbouAssaleh3(self):
		# Problem: (DCSI&#8217;04)
		self.scenarioTestParseRawLine(u"""(2004). <em>Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)</em>. Dalhousie University, Halifax, Nova Scotia, Canada.<br /> [<a href="#Abou-Assaleh2004d" class="toggle">bibtex</a>]   [<a href="http://tony.abou-assaleh.net/papers/abou-assaleh_dcsi04_proceedings.pdf" title="Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)">Proceedings</a>]</div>""",
			{'pubType': u'proceedings',
			 'editor': u'Tony Abou-Assaleh',
 			 'title': u"Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)",
			 'year': u'2004',
			 'address': u'Dalhousie University, Halifax, Nova Scotia, Canada',
			 'url': u'http://tony.abou-assaleh.net/papers/abou-assaleh_dcsi04_proceedings.pdf'},
			 'http://tony.abou-assaleh.net/publications',
			['Tony', 'Abou-Assaleh'])

	def testParseRawLineAbouAssaleh4(self):
		self.scenarioTestParseRawLine(u"""Abou-Assaleh, T., &amp; Ai, W. (2004). <em>Enhancing GNU grep</em> (Final Project Report, Topics in Program Comprehension (CSCI 6306)). Halifax, Nova Scotia, Canada: Faculty of Computer Science, Dalhousie University.<br /> [<a href="#Abou-Assaleh2004a" class="toggle">bibtex</a>]""",
			{'pubType': u'techreport',
			 'author': u'T. Abou-Assaleh and W. Ai',
 			 'title': u'Enhancing GNU grep',
			 'institution': u'Faculty of Computer Science, Dalhousie University',
			 'year': u'2004',
			 'type': u'Final Project Report, Topics in Program Comprehension (CSCI 6306)',
			 'address': u'Halifax, Nova Scotia, Canada'},
			 'http://tony.abou-assaleh.net/publications',
			['Tony', 'Abou-Assaleh'])

	def testParseRawLineJamesAllen1(self):
		self.scenarioTestParseRawLine(u"""Allen, J.F. <A HREF="ftp://ftp.cs.rochester.edu/pub/u/james/allen-time-again-91.ps.gz">"Time and time again:  The many ways to represent time,"</A> Int'l. Jr. of Intelligent Systems 6, 4, 341-356, July 1991.""",
			{'pubType': u'article',
			 'author': u'J.F. Allen',
 			 'title': u'Time and time again: The many ways to represent time',
			 'journal': u"Int'l. Jr. of Intelligent Systems",
			 'year': u'1991',
			 'month': u'July',
			 'volume': u'6',
			 'pages': u'341-356',
			 'number': u'4',
			 'url': 'ftp://ftp.cs.rochester.edu/pub/u/james/allen-time-again-91.ps.gz'},
			 'http://www.cs.rochester.edu/u/james/')

	def testParseRawLineJamesAllen2(self):
		self.scenarioTestParseRawLine(u"""Hinkelman, E. and J.F. Allen. ``Two Constraints on Speech Act Ambiguity.'' In Proc., 27th Meeting of the Assoc. for Computational Linguistics, 1989.""",
			{'pubType': u'inproceedings',
			 'author': u'E. Hinkelman and J.F. Allen',
 			 'title': u'Two Constraints on Speech Act Ambiguity',
			 'booktitle': u'27th Meeting of the Assoc. for Computational Linguistics',
			 'year': u'1989'},
			 'http://www.cs.rochester.edu/u/james/')

	def testParseRawLineJamesAllen3(self):
		# Missing: Morgan Kaufmann
		self.scenarioTestParseRawLine(u"""Allen, J.F. <A HREF="http://www.cs.rochester.edu/u/james/kr91.pdf">Planning as Temporal Reasoning</A>. In Proc., 2nd Principles of Knowledge Representation and Reasoning, Morgan Kaufmann, 1991.""",
			{'pubType': u'inproceedings',
			 'author': u'J.F. Allen',
 			 'title': u'Planning as Temporal Reasoning',
			 'booktitle': u'2nd Principles of Knowledge Representation and Reasoning',
			 'year': u'1991',
			 'url': u'http://www.cs.rochester.edu/u/james/kr91.pdf'},
			 'http://www.cs.rochester.edu/u/james/')

	def testParseRawLineJanAlexandersson1(self):
		self.scenarioTestParseRawLine(u"""<b>Jan Alexandersson</b> <A HREF="http://www.dfki.de/~janal/public_archive/master.ps">Abstract Interpretation of Logic Programs with External Functions - an Implementation</A> Master thesis, Linköping University, Sweden, August 1993""",
			{'pubType': u'mastersthesis',
			 'author': u'Jan Alexandersson',
 			 'title': u'Abstract Interpretation of Logic Programs with External Functions - an Implementation',
			 'school': u'Linköping University',
			 'address': u'Sweden',
			 'year': u'1993',
			 'month': u'August',
			 'url': u'http://www.dfki.de/~janal/public_archive/master.ps'},
			 'http://www.dfki.de/%7Ejanal/publications.html')

	def testParseRawLineJanAlexandersson2(self):
		self.scenarioTestParseRawLine(u"""<b>Jan Alexandersson</b>, Peter Poller, Michael Kipp and Ralf Engel. <A HREF="http://www.dfki.de/service/NLG/IUI-generation/alexpoll2.ps">Multilingual Summary Generation in a Speech--To--Speech Translation System for Multilingual Dialogues</A>In: <I>Proceedings of the International Natural Language Generation Conference 2000 (INLG-2000)</I> (pp. 148--155). Mitzpe Ramon, Israel, June 2000.""",
			{'pubType': u'inproceedings',
			 'author': u'Jan Alexandersson and Peter Poller and Michael Kipp and Ralf Engel',
 			 'title': u'Multilingual Summary Generation in a Speech--To--Speech Translation System for Multilingual Dialogues',
			 'booktitle': u'Proceedings of the International Natural Language Generation Conference 2000 (INLG-2000)',
			 'address': u'Mitzpe Ramon, Israel',
			 'month': u'June',
			 'year': u'2000',
			 'pages': u'148--155',
			 'url': u'http://www.dfki.de/service/NLG/IUI-generation/alexpoll2.ps'},
			 'http://www.dfki.de/%7Ejanal/publications.html')

	def testParseRawLineEytanAdar1(self):
		self.scenarioTestParseRawLine(u"""Adar, Eytan, Daniel Weld, Brian Bershad, and Steven Gribble, "Why We Search: Visualizing and Predicting User Behavior," WWW'07, Banff, Canada, May, 2007 <A HREF="whysearch.html">(details)</A>""",
			{'pubType': u'inproceedings',
			 'author': u'Eytan Adar and Daniel Weld and Brian Bershad and Steven Gribble',
 			 'title': u'Why We Search: Visualizing and Predicting User Behavior',
			 'booktitle': u"WWW'07",
			 'address': u'Banff, Canada',
			 'month': u'May',
			 'year': u'2007'},
			 'http://www.cond.org/publications.html')

	def testParseRawLineEytanAdar2(self):
		self.scenarioTestParseRawLine(u"""Teevan, Jaime, Eytan Adar, Rosie Jones, and Michael Potts, "Information Re-Retrieval: Repeat Queries in Yahoo's Logs," SIGIR'07, Amsterdam, The Netherlands, July 2007. <A HREF="history2.html"> (details)</A>""",
			{'pubType': u'inproceedings',
			 'author': u'Jaime Teevan and Eytan Adar and Rosie Jones and Michael Potts',
 			 'title': u"Information Re-Retrieval: Repeat Queries in Yahoo's Logs",
			 'booktitle': u"SIGIR'07",
			 'address': u'Amsterdam, The Netherlands',
			 'month': u'July',
			 'year': u'2007'},
			 'http://www.cond.org/publications.html')

	def testParseRawLineEytanAdar3(self):
		self.scenarioTestParseRawLine(u"""Adar, Eytan, "SaRAD: A Simple and Robust Abbreviation Dictionary," Bioinformatics, March 1, 2004, 20(4):527-533.""",
			{'pubType': u'article',
			 'author': u'Eytan Adar',
 			 'title': u'SaRAD: A Simple and Robust Abbreviation Dictionary',
			 'journal': u'Bioinformatics',
			 'volume': u'20',
			 'pages': u'527-533',
			 'number': u'4',
			 'month': u'March',
			 'year': u'2004'},
			 'http://www.cond.org/publications.html')

	def testParseRawLineMiguelAAlonso1(self):
		self.scenarioTestParseRawLine(u"""<A HREF="http://www.dc.fi.udc.es/~alonso/" TARGET="_top">Miguel A. Alonso</A>, and <A HREF="http://www.lsi.us.es/~vjdiaz/" TARGET="_top">V&iacute;ctor J. D&iacute;az</A>, <b>Variants of mixed parsing of TAG and TIG</b>, <A HREF="http://www.atala.org/tal/tal.html">Traitement Automatique des Langues</A>, 44(3):41-65, 2003. ISSN 1248-9433.<BR> [<A HREF="http://coleweb.dc.fi.udc.es/cole/library/ps/AloDia2003b.ps.gz">AloDia2003b.ps.gz</A>] [<A HREF="http://coleweb.dc.fi.udc.es/cole/library/ps/AloDia2003b.pdf">AloDia2003b.pdf</A>]""",
			{'pubType': u'article',
			 'author': u'Miguel A. Alonso and Víctor J. Díaz',
 			 'title': u'Variants of mixed parsing of TAG and TIG',
			 'journal': u'Traitement Automatique des Langues',
			 'volume': u'44',
			 'pages': u'41-65',
			 'number': u'3',
			 'year': u'2003',
			 'issn': u'1248-9433',
			 'url': u'http://coleweb.dc.fi.udc.es/cole/library/ps/AloDia2003b.pdf'},
			 'http://www.dc.fi.udc.es/%7Ealonso/papers.html',
			 ['Miguel', 'Alonso'])

	#{'issn': '0302-9743', 'isbn': '978-3-540-75866-2', 'title': 'Generation of indexes for compiling efficient parsers from formal specifications', 'author': 'Carlos Gomez-Rodriguez and Miguel A. Alonso and Manuel Vilares', 'year': '2007', 'pages': '257-264', 'volume': '4739', 'address': 'Springer-Verlag, Berlin-Heidelberg-New York', 'journal': 'Roberto Moreno-Díaz, Franz Pichler, and Alexis Quesada-Arencibia (eds.), Computer Aided Systems Theory'}
	#{'issn': '0302-9743', 'isbn': '978-3-540-75866-2', 'title': 'Generation of indexes for compiling efficient parsers from formal specifications', 'author': 'Carlos Gomez-Rodriguez and Miguel A. Alonso and Manuel Vilares', 'year': '2007', 'pages': '257-264',                   'address': 'Springer-Verlag, Berlin-Heidelberg-New York', 'booktitle': 'volume'}
	#def testParseRawLineMiguelAAlonso2(self):
		## TODO - problem: Carlos G&oacute;mez-Rodr&iacute;guez
		#self.scenarioTestParseRawLine(u"""<a href="http://www.grupocole.org/~cgomezr/">Carlos Gomez-Rodriguez</a>, <A HREF="http://www.dc.fi.udc.es/~alonso/" TARGET="_top">Miguel A. Alonso</A> and <a href="http://www.dc.fi.udc.es/~vilares/">Manuel Vilares</a>, <b>Generation of indexes for compiling efficient parsers from formal specifications</b>, in  Roberto Moreno-Díaz, Franz Pichler, and Alexis Quesada-Arencibia (eds.), <EM>Computer Aided Systems Theory</EM>, volume <A HREF="http://link.springer.de/link/service/series/0558/tocs/t4739.htm">4739</A> of <A HREF="http://www.springer.de/comp/lncs/" TARGET="_top"> <EM>Lecture Notes in Computer Science</EM></A>, pp. 257-264, <A HREF="http://www.springer.de" TARGET="_top">Springer-Verlag</A>, Berlin-Heidelberg-New York, 2007. ISSN 0302-9743 / ISBN 978-3-540-75866-2.<BR> [<A HREF="http://coleweb.dc.fi.udc.es/cole/library/ps/GomAloVil2007b.pdf">GomAloVil2007b.pdf</A>]""",
			#{'pubType': u'article',
			 #'author': u'Carlos Gomez-Rodriguez and Miguel A. Alonso and Manuel Vilares',
 			 #'title': u'Generation of indexes for compiling efficient parsers from formal specifications',
			 #'journal': u'Roberto Moreno-Díaz, Franz Pichler, and Alexis Quesada-Arencibia (eds.), Computer Aided Systems Theory',
			 #'volume': u'4739',
			 #'pages': u'257-264',
			 #'address': u'Springer-Verlag, Berlin-Heidelberg-New York',
			 #'year': u'2007',
			 #'issn': u'0302-9743',
			 #'isbn': u'978-3-540-75866-2'},
			 #'http://www.dc.fi.udc.es/%7Ealonso/papers.html',
			 #['Miguel', 'Alonso'])

	#{'isbn': '0-8186-8616-2', 'title': 'Dynamic Programming as Frame for Efficient Parsing', 'booktitle': "SCCC'98, XVIII International Conference of the Chilean Computer Science Society, November 9-14, 1998. Antofagasta, Chile", 'author': 'Manuel Vilares and David Cabrero and Miguel A. Alonso', 'month': 'November', 'year': '1998', 'pages': '68-75', 'address': 'IEEE Computer Society Press, Los Alamitos, California, USA'}
	#{'isbn': '0-8186-8616-2', 'title': 'Dynamic Programming as Frame for Efficient Parsing', 'booktitle': "SCCC'98", 'author': 'Manuel Vilares and David Cabrero and Miguel A. Alonso', 'number': '14', 'month': 'November', 'year': '1998', 'pages': '68-75', 'address': 'XVIII International Conference of the Chilean Computer Science Society'}
	#def testParseRawLineMiguelAAlonso3(self):
		#self.scenarioTestParseRawLine(u"""<A HREF="http://www.dc.fi.udc.es/~vilares/" TARGET="_top">Manuel Vilares</A>, <A HREF="http://coleweb.dc.fi.udc.es/~cabrero/" TARGET="_top">David Cabrero</A> and <A HREF="http://www.dc.fi.udc.es/~alonso/" TARGET="_top">Miguel A. Alonso</A>, <b>Dynamic Programming as Frame for Efficient Parsing</b>, in <em><a href="http://www.ing.puc.cl/~sccc98" TARGET="_top">SCCC'98, XVIII International Conference of the Chilean Computer Science Society, November 9-14, 1998. Antofagasta, Chile</a></em>, pp. 68-75,     <A HREF="http://computer.org/cspress">IEEE Computer Society Press</A>, Los Alamitos, California, USA, 1998. ISBN 0-8186-8616-2.<br>""",
			#{'pubType': u'inproceedings',
			 #'author': u'Manuel Vilares and David Cabrero and Miguel A. Alonso',
 			 #'title': u'Dynamic Programming as Frame for Efficient Parsing',
			 #'booktitle': u"SCCC'98, XVIII International Conference of the Chilean Computer Science Society, November 9-14, 1998. Antofagasta, Chile",
			 #'pages': u'68-75',
			 #'year': u'1998',
			 #'month': u'November',
			 #'address': u'IEEE Computer Society Press, Los Alamitos, California, USA',
			 #'isbn': u'0-8186-8616-2'},
			 #'http://www.dc.fi.udc.es/%7Ealonso/papers.html',
			 #['Miguel', 'Alonso'])

	def testParseRawLineAlergia(self):
		self.scenarioTestParseRawLine(u"""Alegria I.,&nbsp;Arregi X.,&nbsp;Artola  X.,&nbsp;Díaz de Ilarraza A.,&nbsp;Labaka G.,&nbsp;Lersundi M.,&nbsp;Mayor A.,&nbsp;Sarasola K.&nbsp;2008<br><b>Strategies for sustainable MT for Basque: incremental design, reusability, standardization and open-source</b><br><i>Proceedings of the IJCNLP-08 Workshop on NLP for Less Privileged Languages. pages 59-64. Asian Federation of Natural Language Processing, Hyderabad, India,</i><br>""",
			{'pubType': u'inproceedings',
			 'author': u'I. Alegria and X. Arregi and X. Artola and Díaz de Ilarraza A. and G. Labaka and M. Lersundi and A. Mayor and K. Sarasola',
 			 'title': u'Strategies for sustainable MT for Basque: incremental design, reusability, standardization and open-source',
			 'booktitle': u'Proceedings of the IJCNLP-08 Workshop on NLP for Less Privileged Languages',
			 'pages': u'59-64',
			 'address': u'Asian Federation of Natural Language Processing, Hyderabad, India',
			 'year': u'2008'},
			 'http://ixa.si.ehu.es/Ixa/Argitalpenak/kidearen_argitalpenak?kidea=1000808989',
			 ['Ixa', 'Taldea', 'Argitalpenak', u'Iñaki', 'Alegria'])

	def testParseRawLineAsierAlcazar(self):
		self.scenarioTestParseRawLine(u"""<p class=MsoNormal style='margin-left:1.0in;text-align:justify;text-justify:inter-ideograph;text-indent:-.75in'>2003<span style='mso-tab-count:1'>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp; </span><span lang=EN-GB style='mso-ansi-language:EN-GB'>Verb Classes and Aspectual Interpretation in Basque. </span><span class=GramE><i>Proceedings of </i><i><span lang=IT style='mso-ansi-language:IT'>Console XI.</span></i></span><i><span lang=IT style='mso-ansi-language:IT'> </span></i><i style='mso-bidi-font-style:normal'><span lang=IT style='mso-ansi-language:IT'>Universit&agrave; degli Studi di Padova</span></i><span lang=IT style='mso-ansi-language:IT'>, Padua, Italy.</span><i><o:p></o:p></i></p>""",
			{'pubType': u'inproceedings',
			 'author': u'Asier Alcázar',
 			 'title': u'Verb Classes and Aspectual Interpretation in Basque',
			 'booktitle': u'Proceedings of Console XI.',
			 'address': u'Università degli Studi di Padova, Padua, Italy',
			 'year': u'2003'},
			 'http://www-scf.usc.edu/%7Ealcazar/#_List_of_publications',
			 ['Asier', u'Alcázar'])

	## TODO 30.3.2008
	#def testParseRawLineLadaAdamic(self):
		#self.scenarioTestParseRawLine(u"""<span class="color003366">&raquo;&nbsp;</span> <b><a href="http://www2007.org/papers/paper516.pdf">Expertise Networks in Online Communities: Structure and Algorithms</a></b>,<br>Jun Zhang, Mark Ackerman and Lada Adamic, WWW2007, Banff, Canada. <a href="presentations/expertiserank.ppt"><em>related PPT slides </em></a>""",
			#{'pubType': u'inproceedings',
			 #'author': u'Jun Zhang and Mark Ackerman and Lada Adamic',
 			 #'title': u'Expertise Networks in Online Communities: Structure and Algorithms',
			 #'booktitle': u'WWW2007',
			 #'year': u'2007',
			 #'address': u'Banff, Canada'},
			 #'http://www-personal.umich.edu/%7Eladamic/',
			 #['Lada', 'Adamic', 'School'])

	def testParseRawLineHynekHermansky(self):
		self.scenarioTestParseRawLine(u"""<b>T. Arai, H. Hermansky, M. Pavel and C. Avendano</b>, &quot;Intelligibility of Speech with Filtered Time Trajectories of LPC Cepstrum&quot;, <em>in The Journal of The Acoustical Society of America</em>, vol. 100, num.  4, pp. 2756-,  Dec, 1996, <A HREF=http://www.bme.ogi.edu/~hynek/publications/pdf/takajuki.pdf>pdf</a>, <A HREF=http://www.bme.ogi.edu/~hynek/publications/ps/takajuki.ps.Z>postscript</a>, <A HREF=http://www.bme.ogi.edu/~hynek/publications/abstract/takajuki.txt>abstract</a>.</td></tr><tr><td valign=top>avendano95a""",
			{'pubType': u'article',
			 'author': u'T. Arai and H. Hermansky and M. Pavel and C. Avendano',
 			 'title': u'Intelligibility of Speech with Filtered Time Trajectories of LPC Cepstrum',
			 'journal': u'The Journal of The Acoustical Society of America',
			 'volume': u'100',
			 'number': u'4',
			 'pages': u'2756-',
			 'year': u'1996',
			 'month': u'Dec',
			 'url': u'http://www.bme.ogi.edu/~hynek/publications/pdf/takajuki.pdf'},
			 'http://www.bme.ogi.edu/~hynek/cgi-bin/publications/showbib_asp.pl?all',
			 ['Hynek', 'Hermansky'])

	def testParseRawLineMedunaWork(self):
		self.scenarioTestParseRawLine(u"""<div class="li"> Meduna, A., Horvath, G.: On State Grammars. Acta Cybernetica, p. 4-14, 1988</div>""",
			{'pubType': u'article',
			 'author': u'A. Meduna and G. Horvath',
 			 'title': u'On State Grammars',
			 'journal': u'Acta Cybernetica',
			 'pages': u'4-14',
			 'year': u'1988'},
			 'http://www.fit.vutbr.cz/~meduna/work/doku.php?id=vita:vita', [])

	def testParseRawLineHonzik1(self):
		self.scenarioTestParseRawLine(u"""<b>Honzík Jan M., Zachoval Jan, Zatloukal Josef</b>: <a href="/research/view_pub.php.en?id=5898">Programování - FORTRAN</a>, VUT v Brně, CZ, FE VUT, 1970, <nobr>p.&nbsp;107</nobr>""",
			{'pubType': u'book', # Should be misc
			 'author': u'Honzík Jan M. and Zachoval Jan and Zatloukal Josef',
 			 'title': u'Programování - FORTRAN',
			 'pages': u'107',
			 'location': u'VUT v Brně, CZ',
			 'publisher': u'FE VUT',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=5898',
			 'year': u'1970'},
			 'http://www.fit.vutbr.cz/research/view_pub.php.en?uperson=5&years=99', [])

	def testParseRawLineGrezl1(self):
		self.scenarioTestParseRawLine(u"""Burget Lukáš, Černocký Jan, Glembek Ondřej, Grézl František, Karafiát Martin, Matějka Pavel, Schwarz Petr</b>: <a href="/research/view_pub.php.en?id=8290">Fusion of heterogeneous speaker recognition systems in the STBU submission for the NIST speaker recognition evaluation 2006</a>, In: IEEE Transactions on Audio, Speech, and Language Processing, Vol.&nbsp;2007, No.&nbsp;12, US, <nobr>p.&nbsp;13</nobr>, <nobr>ISSN 1558-7916</nobr>""",
			{'pubType': u'article',
			 'author': u'Burget Lukáš and Černocký Jan and Glembek Ondřej and Grézl František and Karafiát Martin and Matějka Pavel and Schwarz Petr',
 			 'title': u'Fusion of heterogeneous speaker recognition systems in the STBU submission for the NIST speaker recognition evaluation 2006',
			 'journal': u'IEEE Transactions on Audio', # Should be: IEEE Transactions on Audio, Speech, and Language Processing
			 'pages': u'13',
			 'volume': u'2007',
			 'number': u'12',
			 'location': u'and Language Processing, US', # Should be: US
			 'issn': u'1558-7916',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8290',
			 'year': u'2006'},
			 'http://www.fit.vutbr.cz/~grezl/pubs.php.en', ['František', 'Grézl'])

	def testParseRawLineGrezl2(self):
		self.scenarioTestParseRawLine(u"""<b>Zhu Qifeng, Chen Barry, Grézl František, Morgan Nelson</b>: <a href="/research/view_pub.php.en?id=7909">Improved MLP Structures for Data-Driven Feature Extraction for ASR</a>, In: Interspeech'2005 - Eurospeech - 9th European Conference on Speech Communication and Technology, Lisabon, PT, 2005, <nobr>p.&nbsp;4</nobr>, <nobr>ISSN 1018-4074</nobr>""",
			{'pubType': u'article', # Should be: inproceedings
			 'author': u'Zhu Qifeng and Chen Barry and Grézl František and Morgan Nelson',
 			 'title': u'Improved MLP Structures for Data-Driven Feature Extraction for ASR',
			 'journal': u"Interspeech'2005 - Eurospeech - 9th European Conference on Speech Communication and Technology", # Should be: booktitle
			 'pages': u'4',
			 'location': u'Lisabon, PT',
			 'issn': u'1018-4074',
			 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=7909',
			 'year': u'2005'},
			 'http://www.fit.vutbr.cz/~grezl/pubs.php.en', ['František', 'Grézl'])

	#def testParseRawLineVojnar(self):
		#self.scenarioTestParseRawLine(u"""M. Ceska, V. Janousek, and T. Vojnar. Towards Verifying Distributed Systems Using Object-Oriented Petri Nets. In <I>Proc. of 7th International Workshop on Computer Aided Systems Theory---EUROCAST'99</I>, volume 1798 of <I>LNCS</I>, pages 90--104, 2000. Springer-Verlag.""",
			#{'pubType': u'inproceedings',
			 #'author': u'M. Ceska and V. Janousek and T. Vojna',
 			 #'title': u'Towards Verifying Distributed Systems Using Object-Oriented Petri Nets',
			 #'booktitle': u"Proc. of 7th International Workshop on Computer Aided Systems Theory---EUROCAST'99",
			 #'pages': u'90--104',
			 #'volume': u'1798',
			 #'publisher': u'Springer-Verlag',
			 #'year': u'2000'},
			 #'http://www.fit.vutbr.cz/~vojnar/pubs.php.en', ['Tomáš', 'Vojnar'])

############################## getPublicationType ##############################
	def testGetPublicationTypeArticle(self):
		self.assertEqual('article', getPublicationType({
			'author': 'Abou-Assaleh, T., Cercone, N.',
 			'title': 'Relaxed Unification - Proposal',
			'journal': 'Applied Mathematics Letters',
			'year': '2003',
			'volume': '16',
			'pages': '747-751',
			'number': '5'}))

	def testGetPublicationTypeInProceedings(self):
		self.assertEqual('inproceedings', getPublicationType({
			'author': 'Hinkelman, E. and J.F. Allen',
			'title': 'Two Constraints on Speech Act Ambiguity',
			'booktitle': '27th Meeting of the Assoc. for Computational Linguistics',
			'year': '1989'}))

	def testGetPublicationTypeProceedings(self):
		self.assertEqual('proceedings', getPublicationType({
			'editor': 'Tony Abou-Assaleh',
 			'title': "Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)",
			'year': '2004',
			'address': 'Nova Scotia, Canada'}))

	def testGetPublicationTypeBook(self):
		self.assertEqual('book', getPublicationType({
			'author': 'Alexander Meduna',
 			'title': 'Elements of Compiler Design',
			'publisher': 'Taylor & Francis Informa plc',
			'year': '2003'}))

	def testGetPublicationTypeInbook(self):
		self.assertEqual('inbook', getPublicationType({
			'author': 'Alexander Meduna',
			'title': 'Matrix Grammars under Leftmost and Rightmost Restrictions',
			'pages': '243--257',
			'booktitle': 'Mathematical Linguistics and Related Topics',
			'year': '1994',
			'location': 'Bucharest, CZ',
			'publisher': 'unknown'}))

	def testGetPublicationTypeMastersthesis(self):
		self.assertEqual('mastersthesis', getPublicationType({
			'author': 'Jan Alexandersson',
 			'title': 'Abstract Interpretation of Logic Programs with External Functions - an Implementation',
			'year': '1993',
			'school': 'Linköping University'}))

	def testGetPublicationTypeTechreport(self):
		self.assertEqual('techreport', getPublicationType({
			'author': 'Tony Abou-Assaleh and W. Ai',
 			'title': 'Enhancing GNU grep',
			'institution': 'Faculty of Computer Science, Dalhousie University',
			'year': '2004',
			'type': 'Final Project Report, Topics in Program Comprehension (CSCI 6306)',
			'address': 'Halifax, Nova Scotia, Canada'}))

	def testGetPublicationTypeUnpublished(self):
		self.assertEqual('unpublished', getPublicationType({
			'author': 'Johan Millner',
 			'title': 'N-gram Techniques Used in Natural Language Processing',
			'note': 'Unpublished work'}))

	def testGetPublicationTypeUnknownTypeReturnsMisc(self):
		self.assertEqual('misc', getPublicationType({
			'author': 'Abou-Assaleh, T., Cercone, N.',
 			'title': 'Relaxed Unification - Proposal',
			'year': '2003'}))

################################ splitNames #################################
	def testSplitNamesNoName(self):
		self.assertEquals([], splitNames(''))

	def testSplitNamesOneName(self):
		self.assertEquals(['Petr Zemek'], splitNames('Petr Zemek'))

	def testSplitNamesTwoNamesSeparatedByComma(self):
		self.assertEquals([u'Petr Zemek', u'Pavel Jančák'],
			splitNames(u'Petr Zemek, Pavel Jančák'))

	def testSplitNamesTwoNamesSeparatedByAnd(self):
		self.assertEquals([u'Ondra Bláža', u'Míra Duhoslav'],
			splitNames(u'Ondra Bláža and Míra Duhoslav'))

	def testSplitNamesThreeNamesSeparatedByAnd(self):
		self.assertEquals([u'Ondra Bláža', u'Míra Duhoslav', u'Dušan Jára'],
			splitNames(u'Ondra Bláža and Míra Duhoslav and Dušan Jára'))

	def testSplitNamesThreeNamesSeparatedByCommaALotOfWhitespaces(self):
		self.assertEquals(['Andy McWire', 'Dean Winchester', 'Sammy White'],
			splitNames('Andy\tMcWire,   Dean Winchester,\n \tSammy White '))

	def testSplitNamesOneNameNameAndSurnameSeparatedByComma(self):
		self.assertEquals(['Eytan Adar'], splitNames('Adar, Eytan'))

	def testSplitNamesTwoNamesNameAndSurnameSeparatedByComma(self):
		self.assertEquals(['Miguel Alonso', 'Victor Diaz'],
			splitNames('Alonso, Miguel, Diaz, Victor'))

	def testSplitNamesOneNameNameAndSurnameSeparatedByCommaShortName(self):
		self.assertEquals(['J.F. Allen'], splitNames('Allen, J.F.'))

	def testSplitNamesFourNamesNameAndSurnameSeparatedByCommaShortNames(self):
		self.assertEquals(['T. Abou-Assaleh', 'N. Cercone', u'V. Kešelj', 'R. Sweidan'],
			splitNames(u'Abou-Assaleh, T., Cercone, N., Kešelj, V., Sweidan, R.'))

	def testSplitNamesTwoNamesNameAndSurnameSeparatedByCommaShortNames(self):
		self.assertEquals(['E. Hinkelman', 'J.F. Allen'],
			splitNames('Hinkelman, E. and J.F. Allen'))

	def testSplitNamesTwoNamesNameAndSurnameSeparatedByCommaShortNamesReverseOrder(self):
		self.assertEquals(['E. Hinkelman', 'J.F. Allen'],
			splitNames('E., Hinkelman and J.F., Allen'))

	def testSplitNamesTwoNamesShortMiddlenameSeparatedByAnd(self):
		self.assertEquals(['Miguel A. Alonso', 'Victor J. Diaz'],
			splitNames('Miguel A. Alonso and Victor J. Diaz'))

	def testSplitNamesMiddlenameAtTheEnd(self):
		self.assertEquals(['Honzík Jan M.'],
			splitNames('Honzík Jan M.'))

	def testSplitNamesMiddlenameAtTheEndAndOtherName(self):
		self.assertEquals(['Honzík Jan M.', 'Skokanová Jana'],
			splitNames('Honzík Jan M., Skokanová Jana'))

################################# adjustEntry ##################################
	def testAdjustEntry(self):
		self.assertEqual('When I Was a Young Boy', adjustEntry(' \tWhen I Was a Young Boy.\n,'))

################################# adjustNames ################################
	def testAdjustNamesOneName(self):
		self.assertEqual('Tony Abou-Assaleh', adjustNames(
			'Abou-Assaleh, T.', ['Tony', 'Abou-Assaleh'], True))

	def testAdjustNamesOneNameTwoShortcutsOnlyFirstSubstituted(self):
		self.assertEqual('Tony J. Abou-Assaleh', adjustNames(
			'Abou-Assaleh, T.J.', ['Tony', 'Abou-Assaleh'], True))

	def testAdjustNamesOneNameTwoShortcutsBothSubstituted(self):
		self.assertEqual('Tony Juno Abou-Assaleh', adjustNames(
			'Abou-Assaleh, T.J.', ['Tony', 'Juno', 'Abou-Assaleh'], True))

	def testAdjustNamesOneNameThreeShortcutsMiddleSubstituted(self):
		self.assertEqual('T. Viera C. Abou-Assaleh', adjustNames(
			'Abou-Assaleh, T.V.C.', ['Viera', 'Abou-Assaleh'], True))

	def testAdjustNamesOneNameNoWordFromNameAppearsInUsableWords(self):
		self.assertEqual('Erik C. Plankton', adjustNames(
			'Erik C. Plankton', ['Cicero', 'Humir', 'Parias'], True))

	def testAdjustNamesOneNameTwoPossibleSubstitutionsWinsTheNearestToNameThatAppearsInUsableWords(self):
		self.assertEqual(u'Iñaki Alegria', adjustNames(
			'I. Alegria', ['Ixa', 'Taldea', 'Argitalpenak', u'Iñaki', 'Alegria'], True))

	def testAdjustNamesOneNameNoMoreThanOneSameWordInName(self):
		self.assertEqual('Miguel A. Alonso', adjustNames(
			'Miguel A. Alonso', ['Miguel', 'Alonso'], True))

######################### parseInputDataIntoPublications #######################
	def scenarioTestParseInputDataIntoPublications(self, source, sourceName, expectedResult):
		sourceContent = getRawInputData(source)
		result = parseInputDataIntoPublications(sourceContent, sourceName)
		self.assertEqual(expectedResult, result)

	def testParseInputDataIntoPublicationsZendulka(self):
		self.scenarioTestParseInputDataIntoPublications('./test-files/www.fit.vutbr.cz__%7Ezendulka__pubs.php.en',
			'http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en',
			[{'title': u'Distributed Visual Sensor Network Fusion', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8425', 'booktitle': u'4th Joint Workshop on Multimodal Interaction and Related Machine Learning Algorithms', 'author': u'Chmela\u0159 Petr and Zendulka Jaroslav', 'pubType': 'inproceedings', 'location': u'Brno, CZ', 'year': u'2007', 'pages': u'2'},
			 {'publisher': u'ICSP', 'isbn': u'978-0-7695-2932-5', 'author': u'Chmela\u0159 Petr and Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8426', 'booktitle': u'Eighteenth International Workshop on Database and Expert Systems Applications', 'title': u'Visual Surveillance Metadata Management', 'pubType': 'inproceedings', 'location': u'Regensburg, DE', 'year': u'2007', 'pages': u'79-83'},
			 {'publisher': u'SU', 'isbn': u'80-7248-006-7', 'author': u'Weiss Petr and Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8311', 'booktitle': u'Information Systems and Formal Models', 'title': u'Modeling of Services and Service Collaboration in UML 2.0', 'pubType': 'inproceedings', 'location': u'Opava, CZ', 'year': u'2007', 'pages': u'29-36'},
			 {'publisher': u'MUNI', 'isbn': u'80-210-4102-1', 'author': u'Opr\u0161al Zden\u011bk and Fiedler Zden\u011bk and Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8254', 'booktitle': u'DATAKON 2006 Proceedings of the Annual Database conference', 'title': u'Jak z\xedsk\xe1vat a uplat\u0148ovat znalosti p\u0159i rozvoji IS', 'pubType': 'inproceedings', 'location': u'Brno, CZ', 'year': u'2006', 'pages': u'149-158'},
			 {'publisher': u'MARQ', 'isbn': u'80-86840-20-4', 'author': u'Rudolfov\xe1 Ivana and Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8058', 'booktitle': u"Proceedings of 1st International Workshop WFM'06", 'title': u'Clustering of Protein Sequences', 'pubType': 'inproceedings', 'location': u'P\u0159erov, CZ', 'year': u'2006', 'pages': u'71-78'},
			 {'publisher': u'FIT VUT', 'isbn': u'80-214-3287-X', 'author': u'Rychl\xfd Marek and Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8195', 'booktitle': u'MEMICS 2006 Second Doctoral Workshop on Mathematical and Engineering Methods in Computer Science', 'title': u'Distributed Information System as a System of Asynchronous Concurrent Processes', 'pubType': 'inproceedings', 'location': u'Mikulov, CZ', 'year': u'2006', 'pages': u'206-213'},
			 {'publisher': u'MARQ', 'isbn': u'80-86840-19-0', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=8255', 'title': u"Proceedings of 9th International Conference ISIM'06", 'pubType': 'proceedings', 'location': u'Ostrava, CZ', 'year': u'2006', 'pages': u'192', 'editor': u'Zendulka Jaroslav'},
			 {'publisher': u'MARQ', 'isbn': u'80-86840-09-02', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=7788', 'title': u"Proceedings of 8th Spring International Conference ISIM'05", 'pubType': 'proceedings', 'location': u'Ostrava, CZ', 'year': u'2005', 'pages': u'303', 'editor': u'Zendulka Jaroslav'},
			 {'publisher': u'IDEA', 'isbn': u'1-59140-557-2', 'author': u'Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=7786', 'booktitle': u'Encyclopedia of Data Warehousing and Mining', 'title': u'API Standardization Efforts for Data Mining', 'pubType': 'inproceedings', 'location': u'Hershey, US', 'year': u'2005', 'pages': u'39-43'},
			 {'publisher': u'IDEA', 'isbn': u'1-59140-560-2', 'author': u'Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=7671', 'booktitle': u'Encyclopedia of Database Technologies and Applications', 'title': u'Object-Relational Modeling in the UML', 'pubType': 'inproceedings', 'location': u'Hershey, US', 'year': u'2005', 'pages': u'421-426'},
			 {'publisher': u'IDEA', 'isbn': u'1-59140-557-2', 'author': u'Zendulka Jaroslav', 'url': u'http://www.fit.vutbr.cz/research/view_pub.php.en?id=7819', 'booktitle': u'Encyclopedia of Data Warehousing and Mining', 'title': u'Using Standard APIs for Data Mining in Prediction', 'pubType': 'inproceedings', 'location': u'Hershey, US', 'year': u'2005', 'pages': u'1171-1174'}])

	def testParseInputDataIntoPublicationsTonyAbouAssaleh(self):
		self.scenarioTestParseInputDataIntoPublications('./test-files/tony.abou-assaleh.net__Publications',
			'http://tony.abou-assaleh.net/publications',
			[{u'title': u"Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)", u'timestamp': u'2008.03.01', u'address': u'Dalhousie University, Halifax, Nova Scotia, Canada', u'month': u'October', 'pubType': u'proceedings', u'editor': u'Tony Abou-Assaleh', u'file': u'Proceedings:/papers/abou-assaleh_dcsi04_proceedings.pdf:PDF', u'year': u'2004', u'owner': u'taa'},
			 {u'publisher': u'Springer-Verlag', u'timestamp': u'2008.03.05', u'title': u'Combating Spamdexing: Incorporating Heuristics in Link-Based Ranking', u'series': u'Lecture Notes in Computer Science', u'booktitle': u'Algorithms and Models for the Web-Graph: Fourth International Workshop, WAW 2006, Banff, Canada, November 30 - December 1, 2006, Revised Papers', u'author': u'Tony Abou-Assaleh and Tapajyoti Das', u'month': u'May', u'volume': u'4936', 'pubType': u'incollection', u'editor': u'Aiello, William and Broder, Andrei and Janssen, Jeannette and Milios, Evangelos', u'year': u'2008', u'owner': u'taa', u'pages': u'97\u2013106', u'address': u'Berlin Heidelberg'},
			 {u'note': u'Accepted May 2002', u'title': u'Relaxed Unification \u2014 Proposal', u'journal': u'Applied Mathematics Letters', u'author': u'Tony Abou-Assaleh and Nick Cercone', u'publisher': u'New York, NY: Elsevier Science', u'number': u'5', u'month': u'July', u'volume': u'16', 'pubType': u'article', u'file': u'Article:/papers/abou-assaleh_aml.ps:PostScript', u'year': u'2003', u'owner': u'taa', u'pages': u'747-751'},
			 {u'publisher': u'ACM', u'doi': u'http://doi.acm.org/10.1145/1277741.1277979', u'isbn': u'978-1-59593-597-7', u'title': u'Geographic ranking for a local search engine', u'timestamp': u'2008.03.05', u'booktitle': u'SIGIR \u201807: Proceedings of the 30th annual international ACM SIGIR conference on Research and development in information retrieval', u'author': u'Tony Abou-Assaleh and Weizheng Gao', u'month': u'July 23-27', 'pubType': u'inproceedings', u'location': u'Amsterdam, The Netherlands', u'year': u'2007', u'owner': u'taa', u'organization': u'SIGIR', u'pages': u'911\u2013911', u'address': u'New York, NY, USA'},
			 {u'publisher': u'ACM', u'doi': u'http://doi.acm.org/10.1145/1277741.1277980', u'isbn': u'978-1-59593-597-7', u'title': u'Focused ranking in a vertical search engine', u'booktitle': u'SIGIR \u201807: Proceedings of the 30th annual international ACM SIGIR conference on Research and development in information retrieval', u'author': u'Philip O\u2019Brien and Tony Abou-Assaleh', u'month': u'July 23-27', 'pubType': u'inproceedings', u'location': u'Amsterdam, The Netherlands', u'year': u'2007', u'organization': u'SIGIR', u'pages': u'912\u2013912', u'address': u'New York, NY, USA'},
			 {u'publisher': u'ACM', u'doi': u'http://doi.acm.org/10.1145/1242572.1242727', u'isbn': u'978-1-59593-654-7', u'title': u'A Link-Based Ranking Scheme for Focused Search', u'timestamp': u'2008.03.05', u'booktitle': u'Proceedings of the 16th international conference on World Wide Web (WWW2007)', u'author': u'Tony Abou-Assaleh and Tapajyoti Das and Weizheng Gao and Yingbo Miao and Philip O\u2019Brien and Zhen Zhen', u'month': u'May 8-12', 'pubType': u'inproceedings', u'location': u'Banff, Alberta, Canada', u'year': u'2007', u'owner': u'taa', u'pages': u'1125\u20131126', u'address': u'New York, NY, USA'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Jon Doyle and Vlado Ke\u0161elj and Chris Whidden', u'title': u'DalTREC 2005 QA System Jellyfish: Mark-and-Match Approach to Question Answering', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Fourtheenth Text REtrieval Conference (TREC 2005)', u'address': u'Gaithersburg, Maryland', u'month': u'November', 'pubType': u'inproceedings', u'year': u'2005', u'owner': u'taa', u'organization': u'National Institute of Standards and Technology (NIST)'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'A Probabilistic Evaluation Function for Relaxed Unification', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the 29th IEEE Annual International Computer Software and Applications Conference, COMPSAC 2005', u'address': u'Edinburgh, Scotland', u'month': u'July 25-28', 'pubType': u'inproceedings', u'file': u'Paper:/papers/abou-assaleh_compsac2005.pdf:PDF', u'year': u'2005', u'owner': u'taa'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'Question-Answering with Relaxed Unification', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Conference of the Pacific Association for Computational Linguistics, PACLING\u201905', u'address': u'Tokyo, Japan', u'month': u'August 24-27', 'pubType': u'inproceedings', u'file': u'Paper:/papers/abou-assaleh_pacling05.pdf:PDF', u'year': u'2005', u'owner': u'taa'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'Applying HPSG with Relaxed Unification to Question Answering', u'timestamp': u'2008.03.01', u'booktitle': u'MITACS 5th Annual Conference and Atlantic Interchange', u'address': u'Halifax, Nova Scotia, Canada', u'month': u'June', u'note': u'Second Place Award in the Poster Competition', 'pubType': u'inproceedings', u'file': u'Slides:/papers/abou-assaleh_mitacs04.ppt:PowerPoint;Photo:/papers/abou-assaleh_mitacs04.png:PNG', u'year': u'2004', u'owner': u'taa', u'organization': u'Dalhousie University'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'Applying HPSG with Relaxed Unification to Question Answering', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Third Dalhousie Computer Science In-house Conference (DCSI\u201904)', u'address': u'Halifax, Nova Scotia, Canada', u'month': u'October', 'pubType': u'inproceedings', u'year': u'2004', u'owner': u'taa', u'organization': u'Dalhousie University'},
			 {u'author': u'Anthony Cox and Tony Abou-Assaleh and Wei Ai and Vlado Ke\u0161elj', u'title': u'Lexical Source-Code Transformation', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Software Transformation Systems Workshop (STSW\u201904) at the Third International Conference on Generative Programming and Component Engineering (GPCE\u201904)', u'address': u'Vancouver, British Colombia, Canada', u'month': u'October', 'pubType': u'inproceedings', u'file': u'Paper:/papers/abou-assaleh_stsw04.pdf:PDF', u'year': u'2004', u'owner': u'taa'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj and Ray Sweidan', u'title': u'N-gram-based Detection of New Malicious Code', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the 28th IEEE Annual International Computer Software and Applications Conference, COMPSAC 2004', u'address': u'Hong Kong', u'month': u'September 28-30', 'pubType': u'inproceedings', u'file': u'Paper:/papers/abou-assaleh_compsac2004.pdf:PDF', u'year': u'2004', u'owner': u'taa'},
			 {u'title': u'Detection of New Malicious Code Using N-grams Signatures', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Second Annual Conference on Privacy, Security and Trust (PST\u201904)', u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj and Ray Sweidan', u'month': u'October 13-15', 'pubType': u'inproceedings', u'file': u'Slides:/papers/abou-assaleh_pst04.ppt:PowerPoint;Paper:/papers/abou-assaleh_pst04.pdf:PDF', u'year': u'2004', u'owner': u'taa', u'pages': u'193\u2013196', u'address': u'Fredericton, New Brunswick, Canada'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'An Overview of the Theory of Relaxed Unification', u'booktitle': u'Proceedings of the International Conference on Advances in the Internet, Processing, Systems, Interdisciplinary Research, IPSI-2003', u'address': u'Sveti Stefan, Montenegro, Former Yugoslavia', u'month': u'October 5-11', 'pubType': u'inproceedings', u'file': u'Slides:/papers/abou-assaleh_ipsi03-ru.ppt:PowerPoint;Paper:/papers/abou-assaleh_ipsi03-ru.pdf:PDF', u'year': u'2003', u'owner': u'taa'},
			 {u'author': u'Hathai Tanta-ngai and Tony Abou-Assaleh and Sittichai Jiampojamarn and Nick Cercone', u'title': u'Secure Mail Transfer Protocol (SecMTP)', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the International Conference on Advances in the Internet, Processing, Systems, and Interdisciplinary Research, IPSI-2003', u'address': u'Sveti Stefan, Montenegro, Former Yugoslavia', u'month': u'October 5-11', 'pubType': u'inproceedings', u'file': u'Slides:/papers/abou-assaleh_ipsi03-secmtp.ppt:PowerPoint;Paper:/papers/abou-assaleh_ipsi03-secmtp.pdf:PDF:', u'year': u'2003', u'owner': u'taa'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'Towards the Theory of Relaxed Unification', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the 14th International Symposium on Methodologies for Intelligent Systems, ISMIS 2003', u'address': u'Maebashi City, Japan', u'month': u'October 28-31', 'pubType': u'inproceedings', u'file': u'Paper:/papers/abou-assaleh_ismis03.pdf:PDF', u'year': u'2003', u'owner': u'taa'},
			 {u'author': u'Tony Abou-Assaleh and Nick Cercone and Vlado Ke\u0161elj', u'title': u'Expressing Probabilistic Context-Free Grammars in the Relaxed Unification Formalism', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Conference of the Pacific Association for Computational Linguistics, PACLING\u201903', u'address': u'Halifax, Nova Scotia, Canada', u'month': u'August 22-25', 'pubType': u'inproceedings', u'file': u'Slides:/papers/abou-assaleh_pacling03.ppt:PowerPoint;Paper:/papers/abou-assaleh_pacling03.pdf:PDF', u'year': u'2003', u'owner': u'taa', u'organization': u'Dalhousie University'},
			 {u'title': u'Evolution of Recurrent Neural Networks to Control Autonomous Life Agents', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of 2001 Genetic and Evolutionary Computation Conference Workshop Program: Graduate Student Workshop', u'author': u'Tony Abou-Assaleh and Jianna Zhang and Nick Cercone', u'month': u'July 7-11', 'pubType': u'inproceedings', u'editor': u'L. Spector and E. Goodman and A. Wu and W.B. Langdon and H.-M. Voigt and M. Gen and S. Sen and M. Dorigo and S. Pezeshk and M. Garzon and E. Burke', u'file': u'Paper:/papers/abou-assaleh_gecco01-gsw.pdf:PDF:', u'year': u'2001', u'owner': u'taa', u'pages': u' 385\u2013388', u'address': u'San Francisco, California'},
			 {u'publisher': u'Morgan Kaufmann Publishers', u'title': u'Evolution of Recurrent Neural Networks to Control Autonomous Life Agents', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of the Genetic and Evolutionary Computation Conference, GECCO-2001', u'author': u'Tony Abou-Assaleh and Jianna Zhang and Nick Cercone', u'month': u'July 7-11', 'pubType': u'inproceedings', u'editor': u'L. Spector and E. Goodman and A. Wu and W.B. Langdon and H.-M. Voigt and M. Gen and S. Sen and M. Dorigo and S. Pezeshk and M. Garzon and E. Burke', u'file': u'Paper:/papers/abou-assaleh_gecco01.pdf:PDF:', u'year': u'2001', u'owner': u'taa', u'pages': u'903', u'address': u'San Francisco, California'},
			 {u'title': u'Autonomous Life Agent Using Recurrent Neural Networks and Genetic Algorithms', u'timestamp': u'2008.03.01', u'booktitle': u'Proceedings of Late Breaking Papers at the Genetic and Evolutionary Computation Conference, GECCO-2000', u'author': u'Tony Abou-Assaleh and Jianna Zhang', 'pubType': u'inproceedings', u'year': u'2000', u'owner': u'taa', u'pages': u'1\u20135', u'address': u'Las Vegas, Nevada, USA'},
			 {u'school': u'Faculty of Computer Science, Dalhousie University', u'title': u'Applying Head-driven Phrase Structure Grammar With Relaxed Unification to Question Answering', u'timestamp': u'2008.03.01', u'author': u'Abou-Assaleh, Tony', u'month': u'April', 'pubType': u'phdthesis', u'file': u'Slides:/papers/abou-assaleh_phd-proposal05.ppt:PowerPoint', u'year': u'2005', u'owner': u'taa', u'type': u'Ph.D. Thesis Proposal', u'address': u'Halifax, Nova Scotia, Canada'},
			 {u'school': u'School of Computer Science, University of Waterloo', u'title': u'Theory of Relaxed Unification \u2014 Proposal', u'author': u'Tony Abou-Assaleh', u'month': u'December', 'pubType': u'mastersthesis', u'file': u'Thesis:/papers/abou-assaleh_master_thesis.ps:PostScript:', u'year': u'2002', u'owner': u'taa', u'type': u'Master\u2019s Thesis', u'address': u'Waterloo, Ontario, Canada'},
			 {u'title': u'Building a Web Search Engine', u'timestamp': u'2008.03.01', u'author': u'Tony Abou-Assaleh', u'month': u'December', u'note': u'Invited Presentation', u'owner': u'taa', 'pubType': u'techreport', u'year': u'2006', u'howpublished': u'Invited presentation, Burapha University, Chonburi, Thailand', u'organization': u'Burapha University', u'type': u'Invited presentation', u'institution': u'Burapha University', u'address': u'Chonburi, Thailand'},
			 {u'title': u'Grep and Regular Expressions', u'timestamp': u'2008.03.02', u'author': u'Tony Abou-Assaleh', u'month': u'March', 'pubType': u'techreport', u'year': u'2006', u'owner': u'taa', u'type': u'Invited Presentation', u'institution': u'Toronto Linux User Group (TLUG)', u'address': u'Toronto, Ontario'},
			 {u'title': u'Collaborative, Distributed Information Management and Retrieval Architecture for the Enterprises', u'timestamp': u'2008.03.01', u'author': u'Tony Abou-Assaleh and Oliver Baltzer and Chris Jordan and Philip O\u2019Brien and and Hathai Tanta-ngai', u'number': u'CS2005-13', u'month': u'August', 'pubType': u'techreport', u'file': u'Report:/papers/abou-assaleh_CS-2005-13.pdf:PDF:', u'year': u'2005', u'owner': u'taa', u'type': u'Technical Report', u'institution': u'Faculty of Computer Science, Dalhousie University', u'address': u'Halifax, Nova Scotia, Canada'},
			 {u'title': u'Survey of Global Regular Expression Print (GREP) Tools', u'timestamp': u'2008.03.01', u'author': u'Tony Abou-Assaleh and Wei Ai', u'owner': u'taa', u'month': u'March', u'edition': u'Topics in Program Comprehension (CSCI 6306)', 'pubType': u'techreport', u'year': u'2004', u'howpublished': u'Topics in Program Comprehension (CSCI 6306)', u'organization': u'Faculty of Computer Science, Dalhousie University', u'type': u'Topics in Program Comprehension (CSCI 6306)', u'institution': u'Faculty of Computer Science, Dalhousie University', u'address': u'Halifax, Nova Scotia, Canada'},
			 {u'title': u'Enhancing GNU grep', u'timestamp': u'2008.03.01', u'author': u'Tony Abou-Assaleh and Wei Ai', u'month': u'April', 'pubType': u'techreport', u'year': u'2004', u'owner': u'taa', u'type': u'Final Project Report, Topics in Program Comprehension (CSCI 6306)', u'institution': u'Faculty of Computer Science, Dalhousie University', u'address': u'Halifax, Nova Scotia, Canada'},
			 {u'title': u'Evolution of Recurrent Neural Networks to Control Autonomous Life Agents', u'timestamp': u'2008.03.01', u'author': u'Abou-Assaleh, Tony', u'number': u'CS-2001-22', u'month': u'October', 'pubType': u'techreport', u'file': u'Report:/papers/abou-assaleh_CS-2001-22.pdf:PDF:', u'year': u'2001', u'owner': u'taa', u'type': u'Technical Report', u'institution': u'Department of Computer Science, University of Waterloo', u'address': u'Waterloo, Ontario, Canada'},
			 {u'title': u'Autonomous Life Agent Using Recurrent Neural Networks and Genetic Algorithms', u'timestamp': u'2008.03.01', u'author': u'Tony Abou-Assaleh and Jianna Zhang', u'number': u'CS-00-03', u'month': u'November', 'pubType': u'techreport', u'file': u'Report:/papers/abou-assaleh_cs0003.pdf:PDF:', u'year': u'2000', u'owner': u'taa', u'type': u'Technical Report', u'institution': u'Department of Computer Science, Brock University', u'address': u'St. Catharines, Ontario, Canada'}])

########################### parseDataIntoLinesManually #########################
	def testParseDataIntoLinesManuallyEmptyData(self):
		data = ""
		self.assertEqual([], parseDataIntoLinesManually(data))

	def testParseDataIntoLinesManuallyTxtSingleLine(self):
		data = "Chmelař Petr, Zendulka Jaroslav: Distributed Visual Sensor Network Fusion, 4th Joint Workshop on Multimodal Interaction and Related Machine Learning Algorithms, Brno, CZ, 2007, p. 2"
		self.assertEqual([data], parseDataIntoLinesManually(data))

	def testParseDataIntoLinesManuallyTxtTwoPublicationsInMoreLines(self):
		data = "Chmelař Petr, Zendulka Jaroslav: Visual Surveillance Metadata Management, In: Eighteenth International Workshop on Database and Expert Systems Applications,\nRegensburg, DE, ICSP, 2007, p. 79-83, ISBN 978-0-7695-2932-5\n\n" \
			"Weiss Petr, Zendulka Jaroslav: Modeling of Services and Service Collaboration in UML 2.0, In: Information Systems and Formal Models,\nOpava, CZ, SU, 2007, p. 29-36, ISBN 978-80-7248-006-7"
		self.assertEqual('#'.join(data.split('\n\n')).replace('\n', ' ').split('#'), parseDataIntoLinesManually(data))

	def testParseDataIntoLinesManuallyHtmlTwoLines(self):
		data = """Ixa taldea, Elhuyar Fundazioa&nbsp;2007<br>
				<b>Testu-corpusak: ezaugarriak, eraketa eta tresnak</b><br>
		<i>Hizkuntza, komunikazioaren eta teknologiaren garaian. IVAP aldizkaria. ISBN 978-84-7777-316-0</i><br>
		get:
				<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1173895772/publikoak/pdf">pdf</a>

		<br>
					<br><br>

			Alegria I.,&nbsp;Urkia M.&nbsp;2002<br>
				<b>Morfologia Konputazionala. Euskararen morfologiaren deskribapena.</b><br>
		<i>UEU. ISBN: 84-8438-034-3</i><br>
		get:
		<br>

					<br>"""
		usableWords = ['ixa', 'taldea', 'argitalpenak', 'Iñaki', 'Alegria']
		self.assertEqual(["""Ixa taldea, Elhuyar Fundazioa&nbsp;2007<br>
				<b>Testu-corpusak: ezaugarriak, eraketa eta tresnak</b><br>
		<i>Hizkuntza, komunikazioaren eta teknologiaren garaian. IVAP aldizkaria. ISBN 978-84-7777-316-0</i><br>""", """Alegria I.,&nbsp;Urkia M.&nbsp;2002<br>
				<b>Morfologia Konputazionala. Euskararen morfologiaren deskribapena.</b><br>
		<i>UEU. ISBN: 84-8438-034-3</i><br>
		get:
		<br>

					<br>"""], parseDataIntoLinesManually(data, usableWords))

	def testParseDataIntoLinesManuallyHtmlTwoLinesNoUsableWords(self):
		data = """Ixa taldea, Elhuyar Fundazioa&nbsp;2007<br>
				<b>Testu-corpusak: ezaugarriak, eraketa eta tresnak</b><br>
		<i>Hizkuntza, komunikazioaren eta teknologiaren garaian. IVAP aldizkaria. ISBN 978-84-7777-316-0</i><br>
		get:
				<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1173895772/publikoak/pdf">pdf</a>

		<br>
					<br><br>

			Alegria I.,&nbsp;Urkia M.&nbsp;2002<br>
				<b>Morfologia Konputazionala. Euskararen morfologiaren deskribapena.</b><br>
		<i>UEU. ISBN: 84-8438-034-3</i><br>
		get:
		<br>

					<br>"""
		self.assertEqual(["""Ixa taldea, Elhuyar Fundazioa&nbsp;2007<br>
				<b>Testu-corpusak: ezaugarriak, eraketa eta tresnak</b><br>
		<i>Hizkuntza, komunikazioaren eta teknologiaren garaian. IVAP aldizkaria. ISBN 978-84-7777-316-0</i><br>
		get:
				<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1173895772/publikoak/pdf">pdf</a>

		<br>
					<br><br>""", """Alegria I.,&nbsp;Urkia M.&nbsp;2002<br>
				<b>Morfologia Konputazionala. Euskararen morfologiaren deskribapena.</b><br>
		<i>UEU. ISBN: 84-8438-034-3</i><br>
		get:
		<br>

					<br>"""], parseDataIntoLinesManually(data, []))

	def testParseDataIntoLinesManuallyHtmlOneLinesTwoYearsInOnePublication(self):
		data = """<br>

                    Iñaki Alegria, Lluís Màrquez, Kepa Sarasola (ed.)&nbsp;2008<br>
        	<b>Mixing Approaches to Machine Translation. MATMT2008. Proceedings.</b><br>
	<i>Euskal Herriko Unibertsitatea (ISBN 978-612-2224-7)</i><br>
	get:
			<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1202831327/publikoak/pdf">pdf</a>

	<br>"""
		usableWords = ['ixa', 'taldea', 'argitalpenak', 'Iñaki', 'Alegria']
		self.assertEqual(["""Iñaki Alegria, Lluís Màrquez, Kepa Sarasola (ed.)&nbsp;2008<br>
        	<b>Mixing Approaches to Machine Translation. MATMT2008. Proceedings.</b><br>
	<i>Euskal Herriko Unibertsitatea (ISBN 978-612-2224-7)</i><br>
	get:
			<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1202831327/publikoak/pdf">pdf</a>

	<br>"""], parseDataIntoLinesManually(data, usableWords))

	def testParseDataIntoLinesManuallyHtmlThreeLines(self):
		data = """Alegria I.,&nbsp;Díaz de Ilarraza A.,&nbsp;Labaka G.,&nbsp;Lersundi M.,&nbsp;Mayor A.,&nbsp;Sarasola K.&nbsp;2007<br>

        	<b>Transfer-based MT from Spanish into Basque: reusability, standardization and open source</b><br>
	<i>LNCS 4394. 374-384. Cicling 2007. ISBN-10: 3-540-70938-X ISSN 0302-9734</i><br>
	get:
	<br>
                <br><br>
                   Leturia I., Gurrutxaga A., Areta N., Alegria I., Ezeiza A.&nbsp;2007<br>
        	<b>EusBila, a search service designed for the agglutinative nature of Basque</b><br>

	<i>SIGIR2007- iNEWS'07 workshop. ISBN 978-84-690-6978-3</i><br>
	get:
			<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1190627800/publikoak/pdf">pdf</a>
	<br>
                <br><br>
                   Leturia I., Gurrutxaga A., Alegria I., Ezeiza A.&nbsp;2007<br>
        	<b>CorpEus, a ‘web as corpus’ tool designed for the agglutinative nature of Basque</b><br>

	<i>WAC3 2007 (Web as a Corpus) workshop ISBN 978-2-8746-3082-8 ISSN 1783-2845</i><br>
	get:
			<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1190627973/publikoak/pdf">pdf</a>
	<br>
                <br>"""
		usableWords = ['ixa', 'taldea', 'argitalpenak', 'Iñaki', 'Alegria']
		self.assertEqual(["""Alegria I.,&nbsp;Díaz de Ilarraza A.,&nbsp;Labaka G.,&nbsp;Lersundi M.,&nbsp;Mayor A.,&nbsp;Sarasola K.&nbsp;2007<br>

        	<b>Transfer-based MT from Spanish into Basque: reusability, standardization and open source</b><br>
	<i>LNCS 4394. 374-384. Cicling 2007. ISBN-10: 3-540-70938-X ISSN 0302-9734</i><br>
	get:
	<br>
                <br><br>""", """Leturia I., Gurrutxaga A., Areta N., Alegria I., Ezeiza A.&nbsp;2007<br>
        	<b>EusBila, a search service designed for the agglutinative nature of Basque</b><br>

	<i>SIGIR2007- iNEWS'07 workshop. ISBN 978-84-690-6978-3</i><br>""", """get:
			<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1190627800/publikoak/pdf">pdf</a>
	<br>
                <br><br>""", """Leturia I., Gurrutxaga A., Alegria I., Ezeiza A.&nbsp;2007<br>
        	<b>CorpEus, a ‘web as corpus’ tool designed for the agglutinative nature of Basque</b><br>

	<i>WAC3 2007 (Web as a Corpus) workshop ISBN 978-2-8746-3082-8 ISSN 1783-2845</i><br>
	get:
			<a href="http://ixa.si.ehu.es/Ixa/Argitalpenak/Artikuluak/1190627973/publikoak/pdf">pdf</a>
	<br>
                <br>"""], parseDataIntoLinesManually(data, usableWords))


########################### parseRawInputDataIntoLines #########################
	def scenarioTestNumberOfParsedLines(self, dataFile, expectedResult):
		data = getRawInputData(dataFile)
		data = stripAndCondenseWhitespaces(data)
		usableWords = getUsableWords(data)
		data = removeUselessData(data)
		self.assertEqual(expectedResult, len(parseRawInputDataIntoLines(data, usableWords)))

	# http://www.fit.vutbr.cz/%7Ezendulka/pubs.php.en
	def testFitVutbrCzZendulka(self):
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'www.fit.vutbr.cz__%7Ezendulka__pubs.php.en'), 11)

	# http://www.cs.rochester.edu/u/james/
	def testCsRochesterJames(self):
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'www.cs.rochester.edu__u__james'), 24)

	# http://www.dfki.de/%7Ejanal/publications.html
	def testDfkiJanal(self):
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'www.dfki.de__%7Ejanal__publications.html'), 54) # 1 is for matching "In:"

	# http://tony.abou-assaleh.net/Publications
	def testTonyAbouAssaleh(self):
		# Should be 30, it founds also:
		# True <a href='http://altsearchengines.com/2008/03/06/genieknows-local-version-10-is-live/' rel='external nofollow'>Alt Search Engines &raquo; Blog Archive &raquo; GenieKnows Local Version 1.0 is Live!</a> on the post <a href="http://tony.abou-assaleh.net/genieknows/genieknows-local-version-10-is-live#comment-5">GenieKnows Local Version 1.0 is Live!</a>
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'tony.abou-assaleh.net__Publications'), 31)

	# http://www.cond.org/publications
	def testCond(self):
		# Should be 55, but there is a publication with it's name not in "..."
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'www.cond.org__publications.html'), 54)

	## http://www-personal.umich.edu/%7Eladamic/
	#def testPersonalUmichAdamic(self):
		## Should be 21, these two lines are not publications
		## True <p><a href="courses/si544w08/index.html">SI 544 (W'08,F'06)</a><br> Introduction to Statistics and Data Analysis<br> <a href="courses/si601w07/index.html">SI 601 </a>(<a href="http://www-personal.umich.edu/%7Eladamic/courses/si601f07/">F'07</a>)<br> DRAT: data retrieval and analysis techniques <a href="http://www-personal.umich.edu/~ladamic/courses/si508f07"><br> SI 508 </a>(<a href="http://www-personal.umich.edu/~ladamic/courses/si508f07/">F'07</a>)<br> Networks: Theory and Application (MS level) <a href="courses/si544f06/index.html"></a><br> <a href="http://www-personal.umich.edu/%7Eladamic/si708w07/">SI 708 </a>(<a href="http://www-personal.umich.edu/%7Eladamic/courses/si708w07/">W'O7,</a><a href="http://www-personal.umich.edu/%7Eladamic/si708f07/">F'07</a>)</b><br> Networks: Theory and Application (PhD level) <a href="courses/si544f06/index.html"><br> </a><a href="http://www-personal.umich.edu/%7Eladamic/si503w07/"><b>SI 503 (W'07)</b></a><br> Search and Retrieval <br> <a href="courses/si614w06/index.html"><b>SI 614 (W'06)</b></a> -&gt; 508/708<br> Networks: Theory and Application </p> </p> <p><b><a href="http://www.cscs.umich.edu/netseminar">RIW Networks seminar</a></b> (2006-2007) <br> <br>
		## True <b>current</b><br> <hr> <a href="http://www-personal.umich.edu/%7Eshixl">Xiaolin Shi</a> (PhD, CSE)<br> <a href="http://www-personal.umich.edu/~ebakshy">Eytan Bakshy</a> (PhD, SI)<br> Matt Bonner (CSE undergrad) <br> <hr> <b>past</b> <hr> Noor Ali-Hasan (MSI - thesis '06)<br> Dale Hunscher (MSI - thesis '07)
		#self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			#'www-personal.umich.edu__%7Eladamic'), 23)

	## http://www-scf.usc.edu/%7Ealcazar/#_List_of_publications
	#def testScfUscAlcazar(self):
		## Should be 22, but there is a plenty of other stuff than publications which also match...
		#self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			#'www-scf.usc.edu__%7Ealcazar__#_List_of_publications'), 43)

	## http://ixa.si.ehu.es/Ixa/Argitalpenak/kidearen_argitalpenak?kidea=1000808989
	#def testIxaSiEhuEsIxaAlegria(self):
		## Should be 91
		#self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			#'ixa.si.ehu.es__Ixa__Argitalpenak__kidearen_argitalpenak?kidea=1000808989'), 90)

	# http://www.dc.fi.udc.es/%7Ealonso/papers.html
	def testDcFiUdcAlonso(self):
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'www.dc.fi.udc.es__%7Ealonso__papers.html'), 108)

	# http://www.bme.ogi.edu/~hynek/cgi-bin/publications/showbib_asp.pl?all
	def testBmeOgiEduHynek(self):
		self.scenarioTestNumberOfParsedLines(os.path.join('test-files',
			'www.bme.ogi.edu__~hynek'), 83)

############################### removeUselessData ##############################
	def scenarioTestRemoveUselessData(self, data, expectedResult):
		self.assertEqual(expectedResult, removeUselessData(data))

	def testRemoveUselessData(self):
		self.scenarioTestRemoveUselessData(
			'<html><head><title>Test</title></head><body>This is <b>usefull</b>...<code class="bla">This not</code></body><!-- comment\n<b>continues</b> --></html>',
			'This is <b>usefull</b>...')

############################### getUsableWords ##############################
	def scenarioTestGetUsableWords(self, data, expectedUsableWords):
		self.assertEqual(expectedUsableWords, getUsableWords(data))

	def testGetUsableWordsOnlyAuthorNameAndTitles(self):
		self.scenarioTestGetUsableWords('<html><head><title>Doc. Ing. Jaroslav Zendulka, CSc.</title></head><body><h1>Doc. Ing. Jaroslav Zendulka, CSc.</h1></body></html>',
			['Jaroslav', 'Zendulka'])

	def testGetUsableWordsInTitleAuthorNameAndRedundantData(self):
		self.scenarioTestGetUsableWords('<html><head><title>Publications | Tony Abou-Assaleh</title></head><body></body></html>',
			['Tony', 'Abou-Assaleh'])

	def testGetUsableWordsInHeaderAuthorNameAndRedundantData(self):
		self.scenarioTestGetUsableWords("<html><head><title>bla bla bla</title></head><body><h1>Most of Jan Alexandersson's publications</h1></body></html>",
			['Most', 'Alexandersson'])
			# ^ bad... but what, it can't be perfect...

	def testGetUsableWordsMatchOnlyWordsWithFirstLetterUppercase(self):
		self.scenarioTestGetUsableWords('<html><head><title>Petr Zemek, dont match these words</title></head><body></body></html>',
			['Petr', 'Zemek'])

	def testGetUsableWordsOtherTagsInHeader(self):
		self.scenarioTestGetUsableWords('<html><head><title>nothing</title></head><body><H1><FONT COLOR="#7bd4afccb168">Miguel Desira</FONT></H1></body></html>',
			['Miguel', 'Desira'])

class HelperFunctionsTests(TestCase):
######################### stripAndCondenseWhitespaces ##########################

	def testStripAndCondenseWhitespaces(self):
		self.assertEqual("a b c", stripAndCondenseWhitespaces(" 	a  b	c \r\n"))

############################## decodeHtmlEntities ##############################

	def testDecodeHtmlEntities(self):
		self.assertEqual(' <>&"', decodeHtmlEntities('&nbsp;&lt;&gt;&amp;&quot;'))

################################## removeTags ##################################

	def testRemoveTags(self):
		self.assertEqual('Petr Zemek, "How to do stuff", 2007',
			removeTags('Petr Zemek<br>, <a href="index.html" title="Article">"How to do stuff"</a>, <u>2007</u></br>'))

################################### merge ######################################

	def testMergeNothing(self):
		self.assertEqual([], merge())

	def testMergeOneList(self):
		self.assertEqual([1, 2, 3], merge([1, 2, 3]))

	def testMergeTwoEmptyLists(self):
		self.assertEqual([], merge([], []))

	def testMergeTwoNonemptyLists(self):
		self.assertEqual([1, 2, 3], merge([1], [2, 3]))

	def testMergeFourNonemptyLists(self):
		self.assertEqual([1, 2, 3, 4, 5, 6, 7], merge([1], [2, 3], [4], [5, 6, 7]))

	def testMergeTwoNonemptyTuples(self):
		self.assertEqual([1, 2, 3], merge((1,), (2, 3)))

	def testMergeListAndTuple(self):
		self.assertEqual([1, 2, 3], merge([1], (2, 3)))

############################### getBibtexEntries ###############################
	def scenarioTestGetBibtexEntriesNumberOfEntries(self, fileName, expectedNumber):
		data = getRawInputData(fileName)
		data = stripAndCondenseWhitespaces(data)

		self.assertEqual(expectedNumber, len(getBibtexEntries(data)))

	def testGetBibtexEntriesSomeEntries(self):
		self.scenarioTestGetBibtexEntriesNumberOfEntries(os.path.join('test-files',
			'tony.abou-assaleh.net__Publications'), 30)

	def testGetBibtexEntriesNoEntries(self):
		self.scenarioTestGetBibtexEntriesNumberOfEntries(os.path.join('test-files',
			'www.fit.vutbr.cz__%7Ezendulka__pubs.php.en'), 0)

	def testGetBibtexEntriesOneEntryNoKey(self):
		self.assertEqual(
			[('INPROCEEDINGS', u" author = {Ivana Rudolfová and Jaroslav Zendulka}, title = {Clustering of Protein Sequences}, pages = {71--78}, booktitle = {Proceedings of 1st International Workshop WFM'06}, year = {2006}, location = {Přerov, CZ}, publisher = {}, ISBN = {80-86840-20-4}, language = {english},url = {http://www.fit.vutbr.cz/research/view_pub.php?id=8058} ")],
			getBibtexEntries(u"@INPROCEEDINGS{ author = {Ivana Rudolfová and Jaroslav Zendulka}, title = {Clustering of Protein Sequences}, pages = {71--78}, booktitle = {Proceedings of 1st International Workshop WFM'06}, year = {2006}, location = {Přerov, CZ}, publisher = {}, ISBN = {80-86840-20-4}, language = {english},url = {http://www.fit.vutbr.cz/research/view_pub.php?id=8058} }"))

############################### parseBibtexEntry ###############################
	def testParseBibtexEntryNoNestedCurlyBrackets(self):
		self.assertEqual(
			{'pubType': 'proceedings',
			 'title': "Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)",
			 'year': '2004',
			 'editor': 'Tony Abou-Assaleh',
			 'address': 'Dalhousie University, Halifax, Nova Scotia, Canada',
			 'month': 'October',
			 'file': 'Proceedings:/papers/abou-assaleh_dcsi04_proceedings.pdf:PDF',
			 'owner': 'taa',
			 'timestamp': '2008.03.01'},
			parseBibtexEntry(('PROCEEDINGS', "Abou-Assaleh2004d, title = {Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)}, year = {2004}, editor = {Tony Abou-Assaleh}, address = {Dalhousie University, Halifax, Nova Scotia, Canada}, month = {October}, file = {Proceedings:/papers/abou-assaleh_dcsi04_proceedings.pdf:PDF}, owner = {taa}, timestamp = {2008.03.01} ")))

	def testParseBibtexEntryNestedCurlyBrackets(self):
		self.assertEqual(
			{'pubType': 'misc',
			 'author': r'Arnd Ru{\s}mann',
			 'title': 'Dynamic LL(k) Parsing',
			 'year': '2004'},
			parseBibtexEntry(('Misc', r'author = {Arnd Ru{\s}mann}, title = {Dynamic LL(k) Parsing}, year = {2004}')))


class OutputBibTeXMLFunctionsTests(TestCase):
############################# publications2BibTeXML ############################

	def testPublications2BibTeXMLOnePublicationInproceedings(self):
		publication = {'pubType': u'inproceedings',
				 'author': u'Rudolfová Ivana and Zendulka Jaroslav',
				'title': u'Clustering of Protein Sequences',
				'pages': u'71-78',
				'booktitle': u'Proceedings of 1st International Workshop WFM\'06',
				'year': u'2006',
				'location': u'Přerov, CZ',
				'isbn': u'80-86840-20-4',
				'url': u'http://www.fit.vutbr.cz/research/view_pub.php?id=8058'}
		self.assertEqual((u'<?xml version="1.0" encoding="utf-8"?>' +
			u'<file xmlns="http://bibtexml.sf.net/">' +
				u'<entry id="' + generateUniqueIdForPublication(publication) + '">' +
					u'<inproceedings>' +
						u'<isbn>80-86840-20-4</isbn>' +
						u'<title>Clustering of Protein Sequences</title>' +
						u'<url>http://www.fit.vutbr.cz/research/view_pub.php?id=8058</url>' +
						u'<booktitle>Proceedings of 1st International Workshop WFM\'06</booktitle>' +
						u'<author>Rudolfová Ivana and Zendulka Jaroslav</author>' +
						u'<location>Přerov, CZ</location>' +
						u'<year>2006</year>' +
						u'<pages>71-78</pages>' +
					u'</inproceedings>' +
				u'</entry>' +
			u'</file>').encode('utf8'), publications2BibTeXML([publication]))

	def testPublications2BibTeXMLOnePublicationProceedings(self):
		publication = {'pubType': u'proceedings',
			 'editor': u'Tony Abou-Assaleh',
 			 'title': u"Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)",
			 'year': u'2004',
			 'address': u'Nova Scotia, Canada'}
		self.assertEqual((u'<?xml version="1.0" encoding="utf-8"?>' +
			u'<file xmlns="http://bibtexml.sf.net/">' +
				u'<entry id="' + generateUniqueIdForPublication(publication) + '">' +
					u'<proceedings>' +
						u"<title>Proceedings of the 3rd Dalhousie Computer Science In-house Conference (DCSI'04)</title>" +
						u'<address>Nova Scotia, Canada</address>' +
						u'<editor>Tony Abou-Assaleh</editor>' +
						u'<year>2004</year>' +
					u'</proceedings>' +
				u'</entry>' +
			u'</file>').encode('utf8'), publications2BibTeXML([publication]))

	def testPublications2BibTeXMLSpecialsCharacters(self):
		publication = {'pubType': u'inproceedings',
				 'author': u'Pavel Kec"al',
				'title': u"2 > 1 & 1 < 2 '06"}
		self.assertEqual((u'<?xml version="1.0" encoding="utf-8"?>' +
			u'<file xmlns="http://bibtexml.sf.net/">' +
				u'<entry id="' + generateUniqueIdForPublication(publication) + '">' +
					u'<inproceedings>' +
						u'<author>Pavel Kec&quot;al</author>' +
						u"<title>2 &gt; 1 &amp; 1 &lt; 2 '06</title>" +
					u'</inproceedings>' +
				u'</entry>' +
			u'</file>').encode('utf8'), publications2BibTeXML([publication]))

def suite():
	allTests = []
	for testClass in [IOFunctionsTests, Pubs2BibTeXMLTests, HelperFunctionsTests,
			OutputBibTeXMLFunctionsTests]:
		allTests.append(TestSuite(tuple(makeSuite(testClass, 'test'))))
	return TestSuite(tuple(allTests))

def test():
	runner = TextTestRunner()
	runner.run(suite())

if __name__ == '__main__':
	test()

# End of file
