/*
 * Encoding: UTF-8
 * File:     common.cc
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Implementation of some common functions
 */

#include "common.h"

/**
 * Returns true if selected filename is legal
 */
bool is_legal_filename(const string &file_name) {
	return file_name.find("/") == string::npos &&
		file_name != "." &&
		file_name != ".." &&
		file_name != SERVER_FILE_NAME;
}

/**
 * Returns true if selected file is directory
 */
bool is_directory(const string &file_name) {
	DIR *dirp = opendir(file_name.c_str());
	if (dirp) {
		closedir(dirp);
		return true;
	}

	return false;
}

/**
 * Reads file into string (by reference for effectivity)
 */
void read_file(fstream &file, string &file_data) {
	char buffer[INPUT_FILE_BUFFER_SIZE];
	while (file.read(buffer, INPUT_FILE_BUFFER_SIZE - 1)) {
		file_data += string(buffer, file.gcount());
	}
	file_data += string(buffer, file.gcount());
}

/**
 * Returns file name from the string (cuts off path)
 */
string get_only_filename(const string &file) {
	string file_name(file);
	string::size_type last_slash = file.rfind("/");
	if (last_slash != string::npos) {
		file_name.erase(0, last_slash + 1);
	}

	return file_name;
}

/* End of file common.cc */
