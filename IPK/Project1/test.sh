#!/bin/sh
#
# Encoding: UTF-8
# File:     test.sh
# Project:  Webclient
# Date:     16.2.2007
# Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
#
# Testing script for webclient
#
# Compile the program
#
make && {

#
# Run some simple tests
#

# 1st - html document (index.html)
echo "1st - html document (index.html)"
./webclient http://www.fit.vutbr.cz && echo "OK"

# 2nd - image (fit_logo_cz.gif) - explicit port specification
echo "2nd - image (fit_logo_cz.gif) - explicit port specification"
./webclient http://www.fit.vutbr.cz:80/common/img/fit_logo_cz.gif && echo "OK"

# 3rd - text document (pgp-key.txt)
echo "3rd - text document (pgp-key.txt)"
./webclient http://www.stud.fit.vutbr.cz/~xzemek02/pgp-key.txt && echo "OK"

# 4th - redirection (301 Moved Permanently)
echo "4th - redirection (301 Moved Permanently)"
./webclient http://www.stud.fit.vutbr.cz/~xzemek02 && echo "OK"
}
