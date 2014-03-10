#!/usr/bin/env python
# -*- coding: utf-8 -*-
#
# Project: ISJ - HTML pages with publications to BibTeXML convertor
# Author:  Petr Zemek, xzemek02@stud.fit.vutbr.cz
# Date: 9.2.2008
#

"""Script that converts HTML pages with publications to BibTeXML."""

import re
import sys

############################# General constants ################################

DEBUG = False
SEPARATE_AUTHOR_ELEMENTS = False
OUTPUT_ENCODING = 'utf-8'

################################# Exceptions ###################################

class PublicationsParsingError(Exception):
	def __init__(self, errorMessage = ''):
		Exception.__init__(self, errorMessage)

################################## Regexps #####################################

def compileRegexp(regexp, ignoreCase = True):
	flags = re.DOTALL | re.UNICODE
	if ignoreCase:
		flags |= re.IGNORECASE
	return re.compile(regexp, flags)

RE = {
	# Helpers
	'whitespaces': compileRegexp(ur'\s+'),
	'tag': compileRegexp(ur'<.*?>'),
	'number': compileRegexp(ur'[0-9]'),

	# Lines
	'lLines': [compileRegexp(ur'<td.*?>(.+?)</?td>'),
		compileRegexp(ur'<li.*?>(.+?)(?=</?li>|</?[uo]l>|</?p>)')],
	'lNestedLines': [compileRegexp(ur'<p[^>]*>(.+?)(?:</p>|(?=<p[^>]*>))')],

	# Html
	'htmlBody': compileRegexp(ur'<\s*?body.*?>(.*?)<\s*?/\s*?body.*?>'),
	'htmlComments': compileRegexp(ur'<!--.*?-->'),
	'htmlContentCharset': compileRegexp(ur'content=".+?; charset=(.+?)"'),

	# HTTP headers
	'httpContentTypeCharset': compileRegexp(ur'Content-Type: .+?; charset=(.+?)\r\n'),

	# Bibtex
	'bibtexItem': compileRegexp(ur'(?:\s*(\w+)\s*=\s*\{((?:[^{}]*)|(?:[^{}]*\{[^{}]*\}[^{}]*))\},?)+?'),
	'bibtexEntry': compileRegexp(ur'@([a-zA-Z]+)\{(?:[-_a-zA-Z0-9\'"]+\s*,)?((?:\s*\w+\s*=\s*\{.*?\},?)+?\s*)\}'),

	# Usable words
	'uwTitle': compileRegexp(ur'<\s*?title\s*?>(.*?)<\s*?/\s*?title\s*?>'),
	'uwHeader': compileRegexp(ur'<\s*?h1.*?>(.*?)<\s*?/h1.*?>'),
	'uwWord': compileRegexp(ur'[-._;,:]*?([^.;,:]{4,})[-._;,:]*?'),
	'uwUnusableWords': [compileRegexp(ur'publication[s]?'),
		compileRegexp(ur'information[s]?'),
		compileRegexp(ur'school[s]?')],

	# Parsing
	'pCertainData': {
		'isbn': compileRegexp(ur".*?isbn\s*([-\w]+)"),
		'issn': compileRegexp(ur".*?issn\s*(\d{4}-\d{4})"),
		'year': compileRegexp(ur"\W*?((?:19|20)\d\d)\W*(.+)?"),
		 # TODO - {1,10} is good range?
		'number': compileRegexp(ur"^N.{1,10}?(\d+)$"),
		'volume': compileRegexp(ur"^V.{1,10}?(\d+)$"),
		},
	'pTagAHref': compileRegexp(ur'.*?href="?([^ \t\n\r\f\v">]+)"?.*'),
	'pCountry': compileRegexp(ur"^[A-Z]{2}$", ignoreCase = False),
	'pIn': compileRegexp(ur'In(?::\s*|\s+)(.+)'),
	'pInWord': compileRegexp(ur'^In\b.*'),
	'pInShort': compileRegexp(ur'^(?:In[:]?\s*(?:Proc(?:eedings)?[.,]?)?$)|(?:Proceedings\s+(?:of\s+)?)'),
	'pPubTypes': {
		'booktitle': compileRegexp(ur"(?:Proceedings|Conference|Volume|Book|Paper)"),
		'journal': compileRegexp(ur"(?:Journal|Jr\.)")
		},
	'pUppercaseLetter': compileRegexp(ur'^([A-Z])$', ignoreCase = False),
	'pAuthorShort': compileRegexp(ur"^((?:[A-Z]\.)+)(.*)", ignoreCase = False),
	'pAuthorLong': compileRegexp(ur'^((?:[-\w]+|[-\w]+\s+[-\w]+|[-\S]+\s+de\s+[-\S]+\s+(?:[A-Z]\.)*|[-\w]+\s+(?:[A-Z]\.)+(?:\s+[-\w]+)?|(?:[A-Z]\.)+\s+[-\w]+)?(?:\s*and\s+(?:[-\w]+\s+[-\w]+|[-\w]+\s+(?:[A-Z]\.)+(?:\s+[-\w]+)?|(?:[A-Z]\.)+\s+[-\w]+))*\.?)(\s+[^A-Z]+)?$', ignoreCase = False),
	'pEditor': compileRegexp(ur"\s*[(]?(?:editor)[)]?"),
	'pVolumeNumber': compileRegexp(ur'^(\d+)\((\d+)\)$'), # e.g. 16(5)
	'pNumberOnTheEnd': compileRegexp(ur'(?:\s+)?(\d+)$'),
	'pNumber': compileRegexp(ur'^(\d+)$'),
	'pPages': [compileRegexp(ur"(?:p.{1,10}?)?(\d+-+(?:\d+)?)"),
		compileRegexp(ur"^(?:p.{1,10}?)?(\d+?(?:-+\d+)?)$")],
	'pMasterThesis': compileRegexp(ur'^.*?Master\s+Thesis.*?$'),
	'pSchool': compileRegexp(ur'^(.*?(?:University|Faculty).*)$'),
	'pReport': compileRegexp(ur'.*?Report.*?'),
	'pMonth': compileRegexp(ur'^.*?(Jan(?:uary)?|Feb(?:ruary)?|Mar(?:ch)?|Apr(?:il)?|May|June?|' +
		'July?|Aug(?:ust)?|Sep(?:tember)?|Oct(?:ober)?|Nov(?:ember)?|Dec(?:ember)?)(?:(?:\s+.*?)|$)', ignoreCase = False),

	# Other
	'entryAdjust': compileRegexp('^.*?\s+(?:[A-Z]\.)+$', ignoreCase = False),
	}

################################ I/O functions #################################

def printErrorMessage(errorMessage):
	"""Prints selected error message (string) on the standard error."""
	sys.stderr.write('%s: error: %s\n' % (sys.argv[0], errorMessage))

def parseScriptArguments(argv):
	"""Parses script arguments and returns parsed arguments as an object.
	If a help was requested, it will be shown to the standard output
	and this funtion will not return (script will be aborted). Script will
	be aborted also when there is an error during parsing - error message
	will be printed to the standard error.

	argv - argument vector without a script name (e.g. sys.argv[1:])"""
	from optparse import OptionParser
	parser = OptionParser(usage="Usage: %prog [options] [SOURCE...]")

	(options, args) = parser.parse_args(argv)
	options.sources = args
	return options

def openSourceStream(sourceName):
	"""Opens selected source given by its sourceName (string) and
	returns an object of the file type. If the source cannot be opened,
	IOError is raised. sourceName can be either URL, file path or an empty
	string (in that case, this function returns sys.stdin)."""
	# Try to open with urllib (if source is http, ftp, or file URL)
	import urllib
	try:
		if sourceName.startswith('www'):
			return urllib.urlopen('http://' + sourceName)
		else:
			return urllib.urlopen(sourceName)
	except IOError:
		pass
	except OSError:
		pass

	# Try to open with native open function (if source is pathname)
	try:
		return open(sourceName)
	except IOError:
		pass
	except OSError:
		pass

	# Standard input is the only remaining...
	if sourceName == '':
		return sys.stdin

	raise IOError("Source '" + sourceName + "' cannot be opened, " +
 		"skipping.")

def encodeToUnicode(inputData, inputEncoding = None):
	"""Encodes selected inputData (string) from inputEncoding (string) to unicode
	and returns the encoded version. If inputEncoding is None or the input
	string cannot be encoded, it will be returned in unaltered state."""
	encodings = ['utf8', 'latin2', 'latin1', 'ascii']
	if inputEncoding != None:
		# Input encoding was specified, so first try that encoding
		encodings.insert(0, inputEncoding)

	for encoding in encodings:
		try:
			return inputData.decode(encoding)
		except UnicodeError:
			pass

	# Every encoding attempt failed, return the original string
	return inputData

def getRawInputData(sourceName):
	"""Reads all data from the selected source, identified by its name (string),
	and returns them in a string. Returned data will be encoded in utf8
	(if possible)."""
	stream = openSourceStream(sourceName)
	readInput = stream.read()

	# Check whether there are any HTTP headers with encoding specification
	# (if the source was an URL)
	inputEncoding = None
	try:
		encodingFound = RE['httpContentTypeCharset'].search(unicode(stream.info()))
		if encodingFound != None:
			inputEncoding = encodingFound.group(1).lower()
	except UnicodeError:
		pass
	except AttributeError:
		pass

	# Check whether there is encoding specified in read data
	encodingFound = RE['htmlContentCharset'].search(readInput)
	if encodingFound != None:
		inputEncoding = encodingFound.group(1).lower()

	encodedInputData = encodeToUnicode(readInput, inputEncoding)
	if stream != sys.stdin:
		stream.close()

	return encodedInputData

############################# Helper functions #################################

def stripAndCondenseWhitespaces(text):
	"""Removes whitespaces from the beginning of the string and replaces
	all whitespaces with a single space."""
	return RE['whitespaces'].sub(' ', text.strip())

def decodeHtmlEntities(text):
	"""Decodes HTML entities in the given text (string)."""
	# These cases are handled separately because I don't want to have
	# '\xa0' instead of ' ' etc. in the result
	for entity, replacement in [('&nbsp;', ' '), ('&quot;', '"')]:
		text = text.replace(entity, replacement)

	from BeautifulSoup import BeautifulStoneSoup
	return unicode(BeautifulStoneSoup(text,
		convertEntities=BeautifulStoneSoup.HTML_ENTITIES))

def removeTags(text):
	"""Removes all tags from given text (string)."""
	return RE['tag'].sub('', text.strip())

def merge(*sequences):
	"""Merges given sequences into one list. If no arguments are given,
	it returns []."""
	return [item for sequence in sequences for item in sequence]

############################### Main functions #################################

def createPublicationUrl(sourceName, urlFromPublication):
	"""Creates and returns a complete publication URL from the selected
	sourceName (string) and urlFromPublication (string)."""
	from urlparse import urlparse, urlunparse

	PATH_PART_INDEX = 2
	QUERY_PART_INDEX = 4
	FRAGMENT_PART_INDEX = 5

	url = urlparse(urlFromPublication)
	sourceUrl = urlparse(sourceName)
	newUrl = []
	for i in range(0, len(url)):
		if i == PATH_PART_INDEX and not url[i].startswith('/'):
			# Relative path in urlFromPublication, this must be handled
			# differently
			urlPathPart = url[i]
			if urlPathPart.startswith('./'):
				urlPathPart = urlPathPart[2:]
			try:
				path = '/'.join(sourceUrl[i].split('/')[:-1] + [urlPathPart])
			except IndexError:
				newUrl.append(urlPathPart)
			newUrl.append(path)
		elif (i == QUERY_PART_INDEX or i == FRAGMENT_PART_INDEX) and url[i] == '':
			# Do not add the query and fragment part
			newUrl.append(url[i])
		elif url[i] != '':
			newUrl.append(url[i])
		else:
			newUrl.append(sourceUrl[i])
	return urlunparse(tuple(newUrl))

def splitLine(line):
	"""Splits selected line (string) into a list of line parts.
	Line parts are separated either by tags or delimiters like
	',', ':', '<', '``', '/', "'" and '"'."""
	def skipWhitespaces(text, i):
		while text[i].isspace():
			i += 1
		return i

	def stripText(text):
		return text.strip(', \t\n\r\f\v')

	# Pre-process input line by removing useless tags
	for tag in ['div', 'p']:
		line = compileRegexp(ur'</?\s*?' + tag + ur'\s.*?>').sub(' ', line)

	# TODO - <br>?
	textRegexp = compileRegexp(ur'[-._;,:& \t\n\r\f\v\xc2\xbb]*(.+)')

	sLine = []
	try:
		i = 0
		lineLength = len(line)
		while i < lineLength:
			tag = attributes = text = ''
			i = skipWhitespaces(line, i)
			if line[i] == '<':
				# Search the opening tag and get the tag name and attributes
				i += 1 # Read the beginning '>'
				i = skipWhitespaces(line, i)

				while not line[i].isspace() and line[i] != '>':
					tag += line[i]
					i += 1

				while line[i] != '>':
					attributes += line[i]
					i += 1
				attributes = attributes.strip()
				i += 1 # Read the ending '>'

				if tag != 'br':
					# Now search for the ending tag and store the text inside
					while line[i] != '<':
						text += line[i]
						i += 1

					# Skip the '/' and the tag name
					while line[i] != '>':
						i += 1
					i += 1 # Read the ending '>'
			elif line[i] == '`' and line[i-1] == '`':
				# Search until '' is found
				i += 1
				while i < lineLength and not text.endswith("''"):
					text += line[i]
					i += 1
				text = text[:-2]
			elif line[i] == '"':
				# Search until '"' is found
				i += 1
				while i < lineLength and line[i] != '"':
					text += line[i]
					i += 1
				i += 1
			else:
				bracketNestLevel = 0
				beginningWithBracket = False
				beginningI = i
				while i < lineLength:
					while i < lineLength and line[i] not in [',', ':', '<', '`', '/']:
						if line[i] == '(':
							bracketNestLevel += 1
							if i == beginningI:
								beginningWithBracket = True
						elif line[i] == ')':
							bracketNestLevel -= 1
							if beginningWithBracket and bracketNestLevel == 0:
								text = text[1:] # Remove the beginning '('
								i += 1 # Skip ')'
								# Read following redundant characters
								while i < lineLength and line[i] in ['.', ':']:
									i += 1
								break
						text += line[i]
						i += 1
					if i < lineLength and line[i] != '<':
						# If we're still nested, don't throw away that divider
						if bracketNestLevel > 0:
							text += line[i]
						i += 1 # Read the text divider
					if bracketNestLevel == 0 or line[i] == '<':
						break

			textFound = textRegexp.search(text)
			if textFound != None:
				text = stripText(textFound.group(1))

				if text != '':
					realTexts = [text]

					# Try to split the text by dots if we get more fields
					dotSplittedText = text.split('.')
					if len(filter(lambda x: len(x.split(' ')) > 2, dotSplittedText)) > 2:
						realTexts = dotSplittedText

					for realText in realTexts:
						piece = {}
						if tag != '':
							piece['tag'] = tag.lower()
						if attributes != '':
							piece['attributes'] = attributes
						piece['text'] = stripText(realText)
						# If it is a standalone letter, it probably is a name shortcut,
						# so append a dot to it
						if RE['pUppercaseLetter'].match(piece['text']) != None:
							piece['text'] += '.'
						sLine.append(piece)

						# TODO - remove
						#print '|' + piece['text'] + '|'

		# Remove the final character if necessary
		if sLine[-1]['text'].endswith('.'):
			sLine[-1]['text'] = sLine[-1]['text'][:-1]
	except IndexError, e:
		pass

	return sLine

def adjustEntry(entry):
	"""Adjusts selected text (string) by removing redundant characters from the
	beginning and from the end, like spaces, dot, etc. (if necessary)."""
	stripChars = ',."\' \t\n\r\f\v'
	if RE['entryAdjust'].match(entry) != None:
		# Do not remove the ending dot from a name name (like "Miguel A.")
		entry = entry.lstrip(stripChars)
	else:
		entry = entry.strip(stripChars)
	return entry

def splitNames(names):
	"""Splits selected string containing names (authors, editors, ...)
	into a list of names."""
	def joinNameParts(cp):
		if len(cp) == 1:
			splittedPart = cp[0].split(' ')
			if len(splittedPart) == 2 and splittedPart[1].endswith('.'):
				return splittedPart[1] + ' ' + splittedPart[0]
			else:
				return cp[0]
		elif len(cp) == 2:
			if cp[1].endswith('.') or not cp[0].endswith('.'):
				return cp[1] + ' ' + cp[0]

		return ' '.join(cp)

	# Make the string more "parsing friendly"
	names = stripAndCondenseWhitespaces(names)
	if names == '':
		return []

	splittedNames = []
	# Ex: A and B and C
	for rawNames in names.split(' and '):
		# Ex: A, B, C
		nameParts = rawNames.split(', ')
		while len(nameParts) > 0:
			namePartsLength = len(nameParts)
			nameAdded = False
			if namePartsLength > 1:
				if len(nameParts[0].split(' ')) == 1:
					# Ex: Miguel, A.
					if nameParts[1].endswith('.'):
						i = 2
						while i < namePartsLength and nameParts[i].endswith('.'):
							i += 1
						splittedNames.append(joinNameParts(nameParts[:i]))
						nameAdded = True
						nameParts = nameParts[i:]
					# Ex: A., Miguel
					elif nameParts[0].endswith('.'):
						i = 0
						while i < namePartsLength and nameParts[i].endswith('.'):
							i += 1
						splittedNames.append(joinNameParts(nameParts[:i+1]))
						nameAdded = True
						nameParts = nameParts[i+1:]
					# Ex: Edgar, Dijkstra
					elif len(nameParts[0].split(' ')) == 1 and len(nameParts[1].split(' ')) == 1:
						splittedNames.append(joinNameParts(nameParts[:2]))
						nameAdded = True
						nameParts = nameParts[2:]
				# TODO - this can cause problems (Honzik Jan M. -> M. Honzík Jan)
				#elif nameParts[0].endswith('.'):
					## Ex: "Díaz de Ilarraza A." -> "A. Díaz de Ilarraza"
					#splittedNames.append(joinNameParts([nameParts[0].split(' ')[-1]] + \
						#nameParts[0].split(' ')[:-1]))
					#nameAdded = True
					#nameParts.pop(0)

			if not nameAdded:
				splittedNames.append(joinNameParts([nameParts[0]]))
				nameParts.pop(0)

	return splittedNames

def adjustNames(names, usableWords, doSubstitutions = False):
	"""Adjusts selected names (authors, editors, ...) by replacing appropriate
	shortcuts in their names from usableWords (list of strings), but
	only if doSubstitutions is True."""
	def doSubstitution(letter, word, namePart):
			subForWhat = word
			if compileRegexp('\.' + letter + '\.' + '[A-Z]\.').search(namePart) != None:
				subForWhat = ' ' + subForWhat + ' '
			elif compileRegexp('\.' + letter + '\.').search(namePart) != None:
				subForWhat = ' ' + subForWhat
			elif compileRegexp(letter + '\.' + '[A-Z]\.').search(namePart) != None:
				subForWhat = subForWhat + ' '
			return substRegexp.sub(subForWhat, namePart, 1)

	adjustedNames = []
	if doSubstitutions:
		for name in splitNames(names):
			adjustedName = []
			nameParts = name.split(' ')
			for i in xrange(len(nameParts)):
				namePart = nameParts[i]
				usableWordsCopy = usableWords[:]
				namePartChanged = True
				while namePartChanged:
					namePartChanged = False
					j = 0
					while j < len(usableWordsCopy):
						uwFirstLetter = usableWordsCopy[j][0]
						substRegexp = compileRegexp(uwFirstLetter + '\.', ignoreCase = False)
						substFound = substRegexp.search(nameParts[i])
						if substFound != None:
							# Check that the previous or the next name part correspondend
							# to the previous usable word or the next usable word, respectively,
							# to eliminate possibilities that wrong substitution will be done
							# and subsitute only usable words that are not already in the name
							if ((i > 0 and j > 0 and usableWordsCopy[j-1] == nameParts[i-1]) or \
								(i < (len(nameParts)-1) and j < (len(usableWordsCopy)-1) and usableWordsCopy[j+1] == nameParts[i+1])) and \
								usableWordsCopy[j] not in nameParts:
								namePart = doSubstitution(uwFirstLetter, usableWordsCopy[j], namePart)
								namePartChanged = True
								usableWordsCopy.pop(j)
								break
						j += 1

				adjustedName.append(namePart)

			adjustedNames.append(' '.join(adjustedName))
	else:
		adjustedNames = splitNames(names)

	return ' and '.join(adjustedNames)

COUNTRIES = [
	'Afghanistan', 'Albania', 'Algeria', 'American Samoa', 'Andorra', 'Angola',
	'Anguilla', 'Antarctica', 'Antigua And Barbuda', 'Argentina', 'Armenia',
	'Aruba', 'Australia', 'Austria', 'Azerbaijan', 'Bahamas', 'Bahrain',
	'Bangladesh', 'Barbados', 'Belarus', 'Belgium', 'Belize', 'Benin', 'Bermuda',
	'Bhutan', 'Bolivia', 'Bosnia And Herzegovina', 'Botswana', 'Bouvet Island',
	'Brazil', 'Brunei Darussalam', 'Bulgaria', 'Burkina Faso', 'Burundi',
	'Cambodia', 'Cameroon', 'Canada', 'Cape Verde', 'Cayman Islands',
	'Central African Republic', 'Chad', 'Chile', 'China', 'Christmas Island',
	'Cocos Islands', 'Colombia', 'Comoros', 'Congo', 'Congo', 'Cook Islands',
	'Costa Rica', 'Cote D`Ivoire', 'Croatia', 'Cuba', 'Cyprus', 'Czech Republic',
	'Denmark', 'Djibouti', 'Dominica', 'Dominican Republic', 'East Timor',
	'Ecuador', 'Egypt', 'El Salvador', 'England', 'Equatorial Guinea', 'Eritrea',
	'Estonia', 'Ethiopia', 'Falkland Islands', 'Faroe Islands', 'Fiji', 'Finland',
	'France', 'French Guiana', 'French Polynesia', 'French Southern Territories',
	'Gabon', 'Gambia', 'Georgia', 'Germany', 'Ghana', 'Gibraltar', 'Greece',
	'Greenland', 'Grenada', 'Guadeloupe', 'Guam', 'Guatemala', 'Guinea',
	'Guinea-Bissau', 'Guyana', 'Haiti', 'Heard Island And Mcdonald Islands',
	'Vatican', 'Honduras', 'Hong Kong', 'Hungary', 'Iceland', 'India',
	'Indonesia', 'Iran', 'Iraq', 'Ireland', 'Israel', 'Italy', 'Jamaica',
	'Japan', 'Jordan', 'Kazakstan', 'Kenya', 'Kiribati', 'Korea', 'Kuwait',
	'Kyrgyzstan', 'Lao People', 'Latvia', 'Lebanon', 'Lesotho', 'Liberia',
	'Libyan Arab Jamahiriya', 'Liechtenstein', 'Lithuania', 'Luxembourg',
	'Macau', 'Macedonia', 'Madagascar', 'Malawi', 'Malaysia', 'Maldives', 'Mali',
	'Malta', 'Marshall Islands', 'Martinique', 'Mauritania', 'Mauritius',
	'Mayotte', 'Mexico', 'Micronesia', 'Moldova', 'Monaco', 'Mongolia',
	'Montserrat', 'Morocco', 'Mozambique', 'Myanmar', 'Namibia', 'Nauru',
	'Nepal', 'Netherlands', 'Netherlands Antilles', 'New Caledonia',
	'New Zealand', 'New York', 'Nicaragua', 'Niger', 'Nigeria', 'Niue', 'Norfolk Island',
	'Northern Ireland', 'Northern Mariana Islands', 'Norway', 'Oman', 'Pakistan',
	'Palau', 'Palestinian Territory, Occupied', 'Panama', 'Papua New Guinea',
	'Paraguay', 'Peru', 'Philippines', 'Pitcairn', 'Poland', 'Portugal',
	'Puerto Rico', 'Qatar', 'Reunion', 'Romania', 'Russian Federation', 'Rwanda',
	'Saint Helena', 'Saint Kitts', 'Saint Lucia', 'Saint Pierre',
	'Saint Vincent', 'Samoa', 'San Marino', 'Sao Tome And Principe',
	'Saudi Arabia', 'Scotland', 'Senegal', 'Seychelles', 'Sierra Leone',
	'Singapore', 'Slovakia', 'Slovenia', 'Solomon Islands', 'Somalia',
	'South Africa', 'South Georgia', 'Spain', 'Sri Lanka', 'Sudan', 'Suriname',
	'Svalbard And Jan Mayen', 'Swaziland', 'Sweden', 'Switzerland',
	'Syrian Arab Republic', 'Taiwan', 'Tajikistan', 'Tanzania', 'Thailand',
	'Togo', 'Tokelau', 'Tonga', 'Trinidad And Tobago', 'Tunisia', 'Turkey',
	'Turkmenistan', 'Turks And Caicos Islands', 'Tuvalu', 'Uganda', 'Ukraine',
	'United Arab Emirates', 'United Kingdom', 'United States', 'USA',
	'Uruguay', 'Uzbekistan', 'Vanuatu',	'Venezuela', 'Viet Nam', 'Virgin Islands',
	'Wales', 'Wallis And Futuna', 'Western Sahara', 'World', 'Yemen', 'Yugoslavia',
	'Zambia', 'Zimbabwe']
def hasCountry(text):
	"""Returns True if the selected text (string) contains some country name,
	False otherwise."""
	for country in COUNTRIES:
		if text.find(country) != -1:
			return True

	return False

def getPublicationType(publication):
	"""Returns type of the selected publication (string). If the type cannot
	be determined, it returns 'misc'.

	Supported: Article, Book, Inbook, Inproceedings, Mastersthesis, Misc,
	           Proceedings, Techreport, Unpublished
	Unsupported: Booklet, Conference, Incollection, Phdthesis, Manual"""
	p = publication
	# First check keys that are specific for some publication type
	if p.has_key('type'):
		return 'techreport'

	# No specific keys found, try to decide according to present keys
	if (p.has_key('author') or p.has_key('editor')) and p.has_key('title'):
		if p.has_key('year'):
			# Article
			if p.has_key('journal'):
				return 'article'
			# Inbook/book
			elif p.has_key('publisher'):
				if p.has_key('chapter') or (p.has_key('pages') and len(p['pages'].split('-')) > 1):
					return 'inbook'
				else:
					return 'book'
			# Master's thesis
			elif p.has_key('school'):
				return 'mastersthesis'
			# Inproceedings
			elif p.has_key('booktitle'):
				return 'inproceedings'
			# Techreport
			elif p.has_key('institution'):
				return 'techreport'
			# Proceedings
			elif p.has_key('editor'):
				return 'proceedings'

		# Unpublished
		if p.has_key('note'):
			return 'unpublished'

	# No type matches
	return 'misc'

def isValidPublication(p):
	"""Returns True if the selected publication (dictionary) is valid,
	False otherwise."""
	MAX_FIELD_LENGTH = 400

	if not p.has_key('author') and not p.has_key('editor'):
		return False
	if not p.has_key('title'):
		return False
	for key, value in p.items():
		if key != 'pubType' and len(value) > MAX_FIELD_LENGTH:
			return False

	return True

def parseRawLineIntoPublication(rawLine, sourceName = '', usableWords = []):
	"""Parses selected line into a publication (dictionary)
	with keys like in BibTeX + a special key 'pubType' which is the publication type.

	rawLine - input line to be parsed (string)
	sourceName - name of the source from which is the given line (string)
	usableWords - list of usable words (author names)"""
	# Make the input raw line more "parsing friendly"
	line = stripAndCondenseWhitespaces(rawLine)
	line = decodeHtmlEntities(line)

	MAX_NAME_ITEMS = 6

	publication = {}
	publication['pubType'] = None
	sLine = splitLine(line)
	originalSLine = sLine[:]

	try:
		# Try to get volume and number if they're in one field
		for i in xrange(len(sLine)):
			volumeNumberFound = RE['pVolumeNumber'].match(sLine[i]['text'])
			if volumeNumberFound != None:
				publication['volume'] = volumeNumberFound.group(1)
				publication['number'] = volumeNumberFound.group(2)
				sLine.pop(i)
				publication['pubType'] = 'journal'
				break

		# Get data that are easy and clear to get (if they're in the line)
		for regexpType, regexp in RE['pCertainData'].items():
			for i in xrange(len(sLine)):
				if regexp.match(sLine[i]['text']) != None:
					publication[regexpType] = regexp.search(sLine[i]['text']).group(1)
					sLine.pop(i)
					break

		# Pages
		for pagesRegexp in RE['pPages']:
			for i in xrange(len(sLine)):
				pagesFound = pagesRegexp.search(sLine[i]['text'])
				if pagesFound != None:
					publication['pages'] = pagesFound.group(1)
					sLine.pop(i)
					break
			if publication.has_key('pages'):
				break

		# Get other data
		pubChanged = True
		loopCount = 0
		while pubChanged and len(sLine) > 0:
			pubChanged = False

			# Author
			if not publication.has_key('author') and not publication.has_key('editor'):
				authorFound = False
				if len(sLine) >= 2:
					# Check whether the first item contains more than one author
					if len(sLine[0]['text'].split(',')) >= 2:
						publication['author'] = adjustEntry(sLine[0]['text'])
						sLine.pop(0)
						authorFound = True
						pubChanged = True
						continue
					elif len(sLine[0]['text'].split()) >= MAX_NAME_ITEMS:
						# First item has too many items, so it couldn't be
						# a name - it should be a title (author's name will
						# be get from usable words)
						if len(usableWords) > 0:
							publication['author'] = ' '.join(usableWords)
							publication['title'] = adjustEntry(sLine[0]['text'])
							sLine.pop(0)
							authorFound = True
							pubChanged = True
					else:
						while len(sLine) >= 2:
							# Standalone 'and'
							if sLine[0]['text'] == 'and':
								publication['author'] += ' and ' + adjustEntry(sLine[1]['text'])
								sLine = sLine[2:]
								authorFound = True
								pubChanged = True
								continue

							# Check item length
							if len(sLine[0]['text'].split()) >= MAX_NAME_ITEMS:
								# Author name could not be that long...
								break

							# 'Surname, N.', where 'N.' is in a separate item
							authorShortFound = RE['pAuthorShort'].match(sLine[1]['text'])
							if authorShortFound != None:
								author = sLine[0]['text'] + ', ' + authorShortFound.group(1)
								if not publication.has_key('author'):
									publication['author'] = adjustEntry(author)
								else:
									publication['author'] += ', ' + adjustEntry(author)

								authorFound = True
								pubChanged = True

								rest = authorShortFound.group(2)
								if rest != None and adjustEntry(rest) != '':
									# Return the unused part
									sLine = [{'text': adjustEntry(rest)}] + sLine[2:]
								else:
									sLine = sLine[2:]

								continue

							# 'N., Surname', where N. is in a separate item
							author1stFound = RE['pAuthorShort'].match(sLine[0]['text'])
							author2ndPartFound = compileRegexp(ur'^(\w+)(?:,\s*|\s*and\s*|\s*)(.*)$').search(sLine[1]['text'])
							if author1stFound != None and author2ndPartFound != None:
								author1st = author1stFound.group(1)
								author2nd = author2ndPartFound.group(1)

								if not publication.has_key('author'):
									publication['author'] = author1st + ' ' + author2nd
								else:
									publication['author'] += ', ' + author1st + ' ' + author2nd

								rest = author2ndPartFound.group(2)
								if rest != None:
									adjustedRest = adjustEntry(rest)
									if adjustedRest != '':
										# If it is a standalone letter, it probably is a name shortcut,
										# so append a dot to it
										if RE['pUppercaseLetter'].match(adjustedRest) != None:
											adjustedRest += '.'
										# Return the unused part
										sLine = [{'text': adjustedRest}] + sLine[2:]
									else:
										sLine = sLine[2:]
								else:
									sLine = sLine[2:]

								authorFound = True
								pubChanged = True

								continue

							# 'Name Surname', 'Name Surname and Name Surname',
							# 'Surname N. and Surname N.' etc.
							authorLongFound = RE['pAuthorLong'].search(sLine[0]['text'])
							if authorLongFound != None:
								nextAuthor = adjustEntry(authorLongFound.group(1))
								if not publication.has_key('author'):
									publication['author'] = nextAuthor
								else:
									if not nextAuthor.startswith('and '):
										publication['author'] += ','
									publication['author'] += ' ' + nextAuthor

								authorFound = True
								pubChanged = True

								sLine.pop(0)

								continue

							break

				if not authorFound:
					publication['author'] = adjustEntry(sLine[0]['text'])
					sLine.pop(0)
					pubChanged = True

			# Adjust names
			if publication.has_key('author'):
				publication['author'] = adjustNames(publication['author'], usableWords)
				editorFound = RE['pEditor'].search(publication['author'])
				if editorFound != None:
					publication['editor'] = RE['pEditor'].sub('', publication['author'])
					del publication['author']
					publication['pubType'] = 'proceedings'

			# Year - after authors were processed, this is the best choice
			if not publication.has_key('year'):
				# Check the rest of unprocessed data
				for i in xrange(len(sLine)):
					yearFound = RE['pCertainData']['year'].search(sLine[i]['text'])
					if yearFound != None:
						publication['year'] = yearFound.group(1)
						if sLine[i]['text'].find(' ' + publication['year']) == -1:
							rest = yearFound.group(2)
							if rest != None:
								rest = rest.strip()
								if rest != '':
									# Return the unused part
									sLine[i] = {'text': rest.strip()}
							else:
								sLine.pop(i)
						pubChanged = True
						break

			# Location/publisher
			if not publication.has_key('location'):
				for i in xrange(len(sLine)):
					# Try to find a country identifier
					if RE['pCountry'].match(sLine[i]['text']) != None:
						publication['location'] = sLine[i]['text']
						if i > 0:
							publication['location'] = sLine[i - 1]['text'] + \
								', ' + publication['location']
						# Publisher
						if i < (len(sLine) - 1):
							publication['publisher'] = sLine[i + 1]['text']
							sLine.pop(i-1)
						sLine.pop(i-1)
						sLine.pop(i-1)
						pubChanged = True
						break

			# Title
			if not publication.has_key('title'):
				publication['title'] = adjustEntry(sLine[0]['text'])
				if sLine[0].has_key('tag') and sLine[0]['tag'] == 'a':
					url = RE['pTagAHref'].findall(sLine[0]['attributes'])[0]
					publication['url'] = createPublicationUrl(sourceName, url)
				sLine.pop(0)
				pubChanged = True

			# If the title matches proceedings, then the author is
			# (or should be) editor
			if not publication.has_key('editor') and \
				RE['pInShort'].match(publication['title']) != None and \
				publication.has_key('author'):
					publication['editor'] = publication['author']
					del publication['author']
					publication['pubType'] = 'proceedings'

			# Publication appearance (booktitle, journal, ...)
			if not publication.has_key('booktitle') and \
				not publication.has_key('journal') and \
				not publication.has_key('type') and \
				publication['pubType'] != 'proceedings' and \
				publication['pubType'] != 'techreport' and \
				publication['pubType'] != 'mastersthesis':

				# If this is not a first iteration, the first item has to be
				# the publication appearance
				if loopCount > 0:
					if publication.has_key('volume'): # TODO - Is this correct assumption?
						publication['pubType'] = 'article'
						publication['journal'] = sLine[0]['text']
					elif RE['pReport'].search(sLine[0]['text']) != None:
						publication['pubType'] = 'techreport'
						publication['type'] = sLine[0]['text']
					else:
						publication['pubType'] = 'inproceedings'
						publication['booktitle'] = sLine[0]['text']
					sLine.pop(0)
					pubChanged = True

				# Check for "In", "Proceedings" etc. in the splitted line
				if publication['pubType'] == None:
					for i in xrange(len(sLine)):
						if RE['pInShort'].match(sLine[i]['text']) != None and i < (len(sLine) - 1):
							# TODO - refactor
							pubAppearance = 'booktitle'
							publication['pubType'] = 'inproceedings'
							if publication.has_key('issn'): # TODO - Is this correct assumption?
								pubAppearance = 'journal'
								publication['pubType'] = 'article'
							publication[pubAppearance] = ''
							if sLine[i]['text'] == ('Proceedings of'):
								publication[pubAppearance] += sLine[i]['text'] + ' '
							elif RE['pInWord'].match(sLine[i]['text']) == None:
								publication[pubAppearance] += sLine[i]['text']
								sLine.pop(i)
								pubChanged = True
								break
							publication[pubAppearance] += sLine[i+1]['text']
							sLine = sLine[i+2:]
							pubChanged = True
							break

				# Check for publication type in the string
				if publication['pubType'] == None:
					for i in xrange(len(sLine)):
						for pubAppearance, pubTypeRegexp in RE['pPubTypes'].items():
							if pubTypeRegexp.search(sLine[i]['text']) != None:
								inFound = RE['pIn'].match(sLine[i]['text'])
								if inFound != None:
									publication[pubAppearance] = adjustEntry(inFound.group(1))
								else:
									publication[pubAppearance] = adjustEntry(sLine[i]['text'])
								sLine.pop(i)
								# TODO - refactor
								if pubAppearance == 'booktitle':
									publication['pubType'] = 'inproceedings'
								elif pubAppearance == 'journal':
									publication['pubType'] = 'article'
								else:
									publication['pubType'] = pubAppearance
								break
						if publication['pubType'] != None:
							break

				# Master thesis
				if publication['pubType'] == None:
					for i in xrange(len(sLine)):
						if RE['pMasterThesis'].match(sLine[i]['text']) != None:
							sLine.pop(i)
							pubChanged = True
							publication['pubType'] = 'mastersthesis'
							break

					# Check for a school
					if publication['pubType'] == 'mastersthesis':
						for i in xrange(len(sLine)):
							schoolFound = RE['pSchool'].match(sLine[i]['text'])
							if schoolFound != None:
								publication['school'] = adjustEntry(schoolFound.group(1))
								sLine.pop(i)
								pubChanged = True
								break

				# Techreport - check for an institution
				if publication['pubType'] == 'techreport':
					i = 0
					while i < len(sLine):
						schoolFound = RE['pSchool'].match(sLine[i]['text'])
						if schoolFound != None:
							school = schoolFound.group(1)
							if not publication.has_key('institution'):
								publication['institution'] = adjustEntry(school)
							else:
								publication['institution'] += ', ' + adjustEntry(school)
							sLine.pop(i)
							pubChanged = True
							continue
						i += 1

			# Year
			if not publication.has_key('year'):
				# Try to match volume against a year
				if publication.has_key('volume') and RE['pCertainData']['year'].match(publication['volume']):
					publication['year'] = publication['volume']
					pubChanged = True

				# Check whether some already processed information contains a year
				if not publication.has_key('year'):
					for text in [value for value in publication.values() if value != None]:
						yearFound = RE['pCertainData']['year'].search(text)
						if yearFound != None:
							publication['year'] = yearFound.group(1)
							pubChanged = True
							break

				# Check the rest of unprocessed data
				if not publication.has_key('year'):
					for i in xrange(len(sLine)):
						yearFound = RE['pCertainData']['year'].search(sLine[i]['text'])
						if yearFound != None:
							publication['year'] = yearFound.group(1)
							sLine.pop(i)
							pubChanged = True
							break

				# Check the whole original splitted line, there must be something...
				if not publication.has_key('year'):
					for i in xrange(len(originalSLine)):
						yearFound = RE['pCertainData']['year'].search(originalSLine[i]['text'])
						if yearFound != None:
							publication['year'] = yearFound.group(1)
							pubChanged = True
							break

			# Month
			if not publication.has_key('month'):
				for i in xrange(len(sLine)):
					monthFound = RE['pMonth'].search(sLine[i]['text'])
					if monthFound != None:
						publication['month'] = monthFound.group(1)
						sLine.pop(i)
						pubChanged = True
						break

				if not publication.has_key('month'):
					# Check also the original splitted line, there can be something
					monthShouldNotBeThere = publication.get('author', '') + ' ' + \
						publication.get('editor', '') + ' ' + \
						publication.get('title', '') + ' ' + \
						publication.get('booktitle', '') + ' ' + \
						publication.get('journal', '')
					for i in xrange(len(originalSLine)):
						monthFound = RE['pMonth'].search(originalSLine[i]['text'])
						if monthFound != None:
							month = monthFound.group(1)
							if monthShouldNotBeThere.lower().find(month.lower()) == -1:
								publication['month'] = month
								pubChanged = True
								break

			# Volume
			if not publication.has_key('volume'): # and if loopCount > 0:
				if publication.has_key('journal'):
					volumeFound = RE['pNumberOnTheEnd'].search(publication['journal'])
					if volumeFound != None:
						publication['volume'] = volumeFound.group(1)
						publication['journal'] = RE['pNumberOnTheEnd'].sub('', publication['journal'])
						publication['journal'] = adjustEntry(publication['journal'])
						publication['pubType'] = 'article'

			# Number
			if not publication.has_key('number') and publication.has_key('journal'): # and if loopCount > 0:
				for i in xrange(len(sLine)):
					numberFound = RE['pNumber'].match(sLine[i]['text'])
					if numberFound != None:
						publication['number'] = numberFound.group(1)
						sLine.pop(i)
						pubChanged = True
						break

			# Address
			if not publication.has_key('location') and \
				not publication.has_key('address') and \
				loopCount > 0:
				for i in xrange(len(sLine)):
					if hasCountry(sLine[i]['text']):
						publication['address'] = adjustEntry(sLine[i]['text'])
						sLine.pop(i)
						pubChanged = True
						break

			# Address - additions
			if publication.has_key('address'):
				# City (that appears right before country)
				addressCompleted = False
				for i in xrange(len(sLine)):
					for j in xrange(len(originalSLine)):
						if j > 0 and originalSLine[j]['text'].find(publication['address']) != -1 and \
							sLine[i]['text'] == originalSLine[j-1]['text']:
							if publication.has_key('year') and sLine[i]['text'].find(publication['year']) != -1:
								continue
							publication['address'] = sLine[i]['text'] + ', ' + publication['address']
							addressCompleted = True
							sLine.pop(i)
							break
					if addressCompleted:
						break

				# School (university, ...)
				for i in xrange(len(sLine)):
					schoolFound = RE['pSchool'].match(sLine[i]['text'])
					if schoolFound != None:
						school = adjustEntry(schoolFound.group(1))
						try:
							indexInOriginalSLine = originalSLine.index(sLine[i])
							if originalSLine[indexInOriginalSLine+1]['text'] == sLine[i+1]['text']:
								school += ', ' + sLine[i+1]['text']
								sLine.pop(i+1)
						except ValueError:
							pass
						except LookupError:
							pass
						publication['address'] = school + ', ' + publication['address']
						sLine.pop(i)
						pubChanged = True
						break

				# Some other parts of an address
				splittedAddress = publication['address'].split(', ')
				for i in xrange(len(originalSLine)):
					if originalSLine[i]['text'] == splittedAddress[0] and i > 0 and \
						RE['pCertainData']['year'].search(originalSLine[i-1]['text']) == None:
						try:
							sLineAddressPartIndex = sLine.index(originalSLine[i-1])
							publication['address'] = sLine[sLineAddressPartIndex]['text'] + ', ' + publication['address']
						except ValueError:
							pass

			loopCount += 1

		# Post-processing
		if not isValidPublication(publication):
			raise PublicationsParsingError, 'Invalid publication'

		if len(sLine) > 0 and not publication.has_key('url'):
			# Check whether there is some publication url (if it was not previously found)
			for i in xrange(len(sLine)):
				if sLine[i].has_key('attributes'):
					urlFound = RE['pTagAHref'].search(sLine[i]['attributes'])
					if urlFound != None:
						url = urlFound.group(1)
						if compileRegexp(ur'^.+?\.(?:pdf|ps(?:\.gz)?)$').match(url) != None:
							publication['url'] = createPublicationUrl(sourceName,url)
							sLine.pop(i)
							break

		if publication['pubType'] == None and publication.has_key('title') and \
			len(publication['title'].split('.')) > 1 and \
			not publication.has_key('booktitle') and not publication.has_key('journal'):
			# Title probably contains more than a title
			publication['title'], publication['journal'] = publication['title'].split('.', 2)
			for key in ['title', 'journal']:
				publication[key] = adjustEntry(publication[key])

			# Should it be booktitle instead of journal?
			if RE['pInShort'].search(publication['journal']) != None:
				publication['pubType'] = 'inproceedings'
				publication['booktitle'] = publication['journal']
				del publication['journal']

		if publication['pubType'] == None:
			publication['pubType'] = getPublicationType(publication)
	except ValueError, e:
		raise PublicationsParsingError, str(e)
	except LookupError, e:
		raise PublicationsParsingError, str(e)

	return publication

def isPossiblyValidPublicationsLine(line, thoroughTests = True, usableWords = []):
	"""Returns True if the selected line (text) is possibly valid, False
	otherwise.

	thoroughTest - should perform thorough tests or not? (boolean)
	usableWords - list of usable words (author names)"""
	MIN_LINE_LENGTH = 60

	result = len(line) > MIN_LINE_LENGTH

	if thoroughTests:
		 result = result and \
			(compileRegexp(ur".{4,}?(?:19|20)\d\d.*").search(line) != None or \
			compileRegexp(ur".{4,}?(?:, ?.{4,}?| and .{4,}?)*?, ?\".+?\"").search(line) != None or \
			compileRegexp(ur".{4,}?(?:In:|Proceedings|Conference|Volume|Book|Paper|Thesis).*").search(line) != None)
	else:
		result = result and \
			compileRegexp(ur".{4,}?(?:19|20)\d\d.*").search(line) != None

	# TODO - actually use usable words?

	return result

def parseDataIntoLinesByRegexps(data, regexps, nestedRegexps = [], completeTests = True, usableWords = []):
	"""Parses selected data (string) by regexps (list of compiled regexps) to
	a list of lines, where each line is potentially a publication.
	If nestedRegexps (list of compiled regexps is not empty, it will also try
	to split a single line to nested lines by these regexps.

	thoroughTest - should perform thorough tests or not to find out whether some
	               line (probably) contains valid publication? (boolean)
	usableWords - list of usable words (author names)"""
	bestResult = []
	for regexp in regexps:
		lines = regexp.findall(data)
		validLines = []
		for line in lines:
			if isPossiblyValidPublicationsLine(line, completeTests, usableWords):
				if nestedRegexps != []:
					# Check if the matched line contains some nested publication lines
					nestedLines = parseDataIntoLinesByRegexps(line, nestedRegexps, [], usableWords)
					if len(nestedLines) > 1:
						map(lambda l: validLines.append(l), nestedLines)
						continue
				validLines.append(line)
		if len(validLines) > len(bestResult):
			bestResult = validLines

	return bestResult

def parseDataIntoLinesManually(data, usableWords = []):
	"""Parses selected data (string) "manually", trying to find valid publication
	lines, and returns them in a list of strings."""
	def findStartOfLine(data, yearPos, usableWords):
		data = data[:yearPos]

		if usableWords != []:
			usableWordPos = 0
			for word in usableWords:
				wordFound = compileRegexp(r'.*(' + word + r')[^/]').search(data)
				if wordFound != None:
					if usableWordPos == None:
						usableWordPos = wordFound.start(1)
					elif wordFound.start(1) > usableWordPos:
						usableWordPos = wordFound.start(1)
			return data.rfind('>', 0, usableWordPos) + 1
		else:
			# TODO - this is not a robust solution... but works in simple cases
			return data.rfind('>', 0, len(data)) + 1

	hasHtmlTags = RE['tag'].search(data) != None

	lines = []
	while True:
		yearFound = RE['pCertainData']['year'].search(data)
		if yearFound == None:
			break
		yearPos = yearFound.start(1)

		if hasHtmlTags:
			# Html
			lineStart = findStartOfLine(data, yearPos, usableWords)
			while True:
				# Find line end
				lineEnd = len(data)
				nextDataStart = yearPos + 4
				nextData = data[nextDataStart:]
				nextYearFound = RE['pCertainData']['year'].search(nextData)
				if nextYearFound != None:
					lineEnd = nextDataStart + findStartOfLine(nextData,
						nextYearFound.start(1), usableWords)
					if (findStartOfLine(data, lineEnd, usableWords)) == lineStart:
						yearPos = nextDataStart + nextYearFound.start(1)
						continue
					else:
						lineEnd = findStartOfLine(data, lineEnd, usableWords)
				break
			line = data[lineStart:lineEnd].strip()
			if isPossiblyValidPublicationsLine(line, True, usableWords):
				lines.append(line)
			data = data[lineEnd:]
		else:
			# Plain text, delimiter will be an empty line
			lineStart = data.rfind('\n\n', 0, yearPos) + 1
			lineEnd  = data.find('\n\n', yearPos)
			if lineEnd == -1:
				lineEnd = len(data)
			lines.append(data[lineStart:lineEnd].replace('\n', ' ').strip())
			data = data[lineEnd:]
	return lines

def parseRawInputDataIntoLines(data, usableWords = []):
	"""Parses given data (text) into lines (list of strings) that possibly contain
	some publication information.

	usableWords - list of usable words (author names)"""
	MIN_LINES_BY_REGEXP = 5

	# Try to parse data by regular expressions
	lines = parseDataIntoLinesByRegexps(data, RE['lLines'], RE['lNestedLines'], True, usableWords)
	if len(lines) == 0:
		# Try to parse the data with nested lines regexps, which can be slower,
		# so it is not done until necessary
		lines = parseDataIntoLinesByRegexps(data, RE['lNestedLines'], [], False, usableWords)

	# Try to parse data "manually", "char by char"
	if len(lines) < MIN_LINES_BY_REGEXP:
		lines = parseDataIntoLinesManually(data, usableWords)

	return lines

def getUsableWords(data):
	"""Gets usable words (from title and h1 header) from given data (string)
	and returns them in a list of strings."""
	titleWords = []
	foundTitle = RE['uwTitle'].search(data)
	if foundTitle != None:
		titleWords = filter(lambda w: RE['uwWord'].match(w) != None,
			removeTags(foundTitle.group(1)).split())

	headerWords = []
	foundHeader = RE['uwHeader'].search(data)
	if foundHeader != None:
		headerWords = filter(lambda w: RE['uwWord'].match(w) != None,
			removeTags(foundHeader.group(1)).split())

	usableWords = []
	for words in [titleWords, headerWords]:
		for word in words:
			word = RE['uwWord'].search(word).group(1)
			for unusableWordRegexp in RE['uwUnusableWords']:
				if unusableWordRegexp.search(word) != None or not word[0].isupper():
					break
				if word.endswith("'s"):
					word = word[:-2]
				if word not in usableWords:
					usableWords.append(word)

	return usableWords

def removeUselessData(data):
	"""Removes all useless data (useless tags etc.) from given data (string)
	and returns modified data."""
	# Get only the content of <body></body>
	bodyFound = RE['htmlBody'].search(data)
	if bodyFound != None:
		data = bodyFound.group(1)

	# Remove comments
	data = RE['htmlComments'].sub(' ', data)

	# Remove useless tags, like <code></code>, etc.
	uselessTags = [ur'h.', 'th', 'code']
	for uselessTag in uselessTags:
		data = compileRegexp(ur'<\s*?' + uselessTag + ur'\s.*?>.*?<\s*?/\s*?' + \
			uselessTag + ur'.*?>').sub('', data)

	return data

def getBibtexEntries(data):
	"""Returns bibtex entries (list of strings) from the passed data (string)."""
	data = removeTags(data)
	data = decodeHtmlEntities(data)
	data = stripAndCondenseWhitespaces(data)
	return RE['bibtexEntry'].findall(data)

def parseBibtexEntry(bibtexEntry):
	"""Parses selected bibtexEntry (tuple(publication type as string, items as
	string)) and returns publication (see parseRawLineIntoPublication() for
	details about the format of a publication)."""
	try:
		publication = {}
		if bibtexEntry != None:
			publication['pubType'] = bibtexEntry[0].lower()
			for option, value in RE['bibtexItem'].findall(bibtexEntry[1]):
				publication[option] = value
		return publication
	except ValueError, e:
		raise PublicationsParsingError, str(e)
	except LookupError, e:
		raise PublicationsParsingError, str(e)

def parseInputDataIntoPublications(data, sourceName = ''):
	"""Parses given data (string) into a list of publications (list of dictionaries,
	every dictionary have keys according to BibTeX).

	sourceName - name of the data source"""
	data = stripAndCondenseWhitespaces(data)
	usableWords = getUsableWords(data)

	# Check whether there are any bibtex entries to get better results
	bibtexEntries = getBibtexEntries(data)
	if len(bibtexEntries) > 0:
		try:
			return map(parseBibtexEntry, bibtexEntries)
		except PublicationsParsingError, e:
			pass

	# Standard parsing
	data = removeUselessData(data)

	publications = []
	for line in parseRawInputDataIntoLines(data, usableWords):
		try:
			publications.append(parseRawLineIntoPublication(line, sourceName, usableWords))
		except PublicationsParsingError, e:
			if DEBUG:
				sys.stderr.write(str(e))
			pass
	return publications

######################### Output BibTeXML functions ############################

def generateUniqueIdForPublication(publication):
	"""Generates and returns a unique id (string) for the selected publication.
	Multiple calls for the same publication will return the same id."""
	return str(abs(hash(unicode(publication))))

def publications2BibTeXML(publications, prettyXml = False):
	"""Converts given list of publications into BibTeXML (string).

	prettyXml - should the output be formatted? (bool)"""
	from xml.dom import minidom, XML_NAMESPACE

	doc = minidom.Document()
	fileElement = doc.createElement('file')
	fileElement.setAttributeNS(XML_NAMESPACE, 'xmlns', 'http://bibtexml.sf.net/')
	doc.appendChild(fileElement)

	for publication in publications:
		# Create a copy (because of modifications of the publication)
		pubCopy = publication.copy()

		entryElement = doc.createElement('entry')
		entryElement.setAttribute('id', generateUniqueIdForPublication(publication))
		pubTypeElement = doc.createElement(pubCopy['pubType'])
		del pubCopy['pubType']

		if SEPARATE_AUTHOR_ELEMENTS:
			# Each author will be in a separate element
			nameType = 'editor' if pubCopy.has_key('editor') else 'author'
			for name in splitNames(pubCopy[nameType]):
				nameElement = doc.createElement(nameType)
				nameName = doc.createTextNode(name)
				nameElement.appendChild(nameName)
				pubTypeElement.appendChild(nameElement)
			del pubCopy[nameType]

		# Other elements
		for name, value in pubCopy.items():
			nameElement = doc.createElement(name)
			valueElement = doc.createTextNode(value)
			nameElement.appendChild(valueElement)
			pubTypeElement.appendChild(nameElement)

		entryElement.appendChild(pubTypeElement)
		fileElement.appendChild(entryElement)

	return doc.toprettyxml(encoding=OUTPUT_ENCODING) if prettyXml \
		else doc.toxml(encoding=OUTPUT_ENCODING)

#################################### Main ######################################

def main():
	try:
		options = parseScriptArguments(sys.argv[1:])
		if options.sources == []:
			options.sources = [''] # Read input data from stdin

		publications = []
		for sourceName in options.sources:
			try:
				inputData = getRawInputData(sourceName)
				publications = merge(publications, parseInputDataIntoPublications(
					inputData, sourceName))
			except IOError, e:
				printErrorMessage(str(e))

		sys.stdout.write(publications2BibTeXML(publications, DEBUG))
	except Exception, e:
		if DEBUG:
			raise e
		else:
			printErrorMessage(str(e))

if __name__ == '__main__':
	main()

# End of file
