#!/usr/bin/env python
# -*- coding: utf-8 -*-

from random import randint

order = 100

def main():
	output = ""
	output += "#!/bin/bash\n"
	for i in xrange(1, 11):
		n = i * order
		output += "echo %d\n" % n
		output += "time ./x <<< \"%d " % n
		for j in xrange(0, n):
			output += "%d " % randint(0, 1)
		output += "\" > /dev/null\n"
	print(output)

if __name__ == "__main__":
	main()
