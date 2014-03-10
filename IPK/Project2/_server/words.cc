/*
 * Encoding: UTF-8
 * File:     words.cc
 * Project:  Excercises
 * Date:     25.1.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: How to read words from stdin, sort them and print them to
 *              stdout in 1 declaration and 1 statement ;]
 *
 */

#include <iostream>
#include <algorithm>
#include <iterator>
#include <string>
#include <set>

using namespace std;

int main() {
	multiset<string> words((istream_iterator<string>(cin)),
		(istream_iterator<string>()));

	copy(words.begin(), words.end(),
		ostream_iterator<string>(cout, "\n"));
}

/* End of file words.cc */
