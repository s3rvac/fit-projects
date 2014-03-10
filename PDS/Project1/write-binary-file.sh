#!/usr/bin/python
#-*- coding: utf-8 -*-
#
# Writes a binary file
#

import struct

numberOfBytes = 104
filename = "test-" + str(numberOfBytes) + ".bin"

# Open in binary mode for portability
f = open(filename, "wb")

# Write length field
f.write(struct.pack('i', numberOfBytes))

# Write data
for i in xrange(0, numberOfBytes / 4):
	f.write(struct.pack('i', i))

# Write reference output data
f2 = open(filename + ".ref", "wb")
for i in xrange(0, numberOfBytes / 4):
	f2.write(struct.pack('i', i))

"""
numberOfBytes = 108
filename = "test-" + str(numberOfBytes) + ".bin"

# Open in binary mode for portability
f = open(filename, "wb")

# Write length field
f.write(struct.pack('c', 'l'))

# Write data
for i in xrange(0, numberOfBytes):
	f.write(struct.pack('c', 'A'))
"""

# End of file
