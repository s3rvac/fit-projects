/**
 * Encoding: UTF-8
 * Project:  Serial-TCP/IP bridge
 *
 * @file    io.cc
 * @author  Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * @date    2009
 * @brief   I/O functions implementation.
 *
 */

#include <iostream>
#include <istream>
#include <string>

#include "io.h"
#include "debug.h"

using namespace std;

namespace {
	const size_t BUFFER_SIZE = 4096;
}

string readAllStreamContent(istream &stream) {
	char buffer[BUFFER_SIZE];
	string content;

	while (stream.read(&buffer[0], BUFFER_SIZE - 1)) {
		content += string(buffer, stream.gcount());
	}
	content += string(buffer, stream.gcount());

	return content;
}

void printErrorMessage(const char *errorMessage) {
	cerr << "bridge: " << errorMessage << endl;
}

void printErrorMessage(const string &errorMessage) {
    printErrorMessage(errorMessage.c_str());
}

void printHelp(const string &progName, ostream &stream) {
	stream <<
		"Name: \n"
		"    bridge - Serial-TCP/IP bridge\n"
		"Synopsis:\n"
		"    " << progName << " -f CONFIGURATION_FILE_PATH\n"
		"Description:\n"
		"    Simple serial-TCP/IP bridge. This program starts a concurrent\n"
		"    TCP/IP server and accepts client connections. When some client\n"
		"    sends a message to the server, it redirects it to the serial port.\n"
		"    When there is an incoming message on the serial port, it resends\n"
		"    that message to all connected clients.\n"
		"Examples:\n"
		"    " << progName << " -f bridge.cfg\n"
		"Author:\n"
		"    Petr Zemek, xzemek02@stud.fit.vutbr.cz, 2009"
		<< endl;
}

// End of file io.cc
