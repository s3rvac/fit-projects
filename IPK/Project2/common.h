/*
 * Encoding: UTF-8
 * File:     common.h
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Common headers, declarations for both, client and server
 *              and exceptions
 */

#ifndef __COMMON_H__
	#define __COMMON_H__

/*
 * Debugging
 */
#ifndef NDEBUG
	#define NDEBUG
#endif
#include <cassert>
const unsigned MAX_DBG_MESSAGE_LENGTH = 50;

#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <string>
#include <cctype>
#include <cstring>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <dirent.h>
#include <sys/param.h>
#include <arpa/inet.h>
#include <signal.h>
#include "Protocol.h" /* Used protocol */
#include "Client.h" /* Client class */
#include "Server.h" /* Server class */

using std::cout;
using std::cerr;
using std::endl;
using std::flush;
using std::string;
using std::copy;
using std::ios;
using std::getline;
using std::fstream;
using std::exception;
using std::stringstream;

const string CLIENT_FILE_NAME = "client";
const string SERVER_FILE_NAME = "server";
const size_t INPUT_FILE_BUFFER_SIZE = 4096;

const int MIN_PORT = 1024;
const int MAX_PORT = 65535;

/**
 * Returns true if selected port is legal
 */
static inline bool is_legal_port(int port) {
	return (port >= MIN_PORT && port <= MAX_PORT);
}

/**
 * Returns true if selected filename is legal
 */
bool is_legal_filename(const string &file_name);

/**
 * Returns true if selected file is directory
 */
bool is_directory(const string &file_name);

/**
 * Reads file into string (by reference for effectivity)
 */
void read_file(fstream &file, string &file_data);

/**
 * Returns file name from the string (cuts off path)
 */
string get_only_filename(const string &file);

/*
 * Exceptions
 */
class BasicException: public exception {
private:
	const string s;
public:
	BasicException(const string &e) throw(): s(e) {}
	virtual ~BasicException() throw() {}
	virtual const char *what() throw() { return s.c_str(); }
};

class ParamException: public BasicException {
public:
	ParamException(const string &e) throw(): BasicException(e) {}
};
class NetworkException: public BasicException {
public:
	NetworkException(const string &e) throw(): BasicException(e) {}
};
class RuntimeException: public BasicException {
public:
	RuntimeException(const string &e) throw(): BasicException(e) {}
};
class ProcessException: public BasicException {
public:
	ProcessException(const string &e) throw(): BasicException(e) {}
};

#endif /* #ifndef __COMMON_H__ */

/* End of file common.h */
