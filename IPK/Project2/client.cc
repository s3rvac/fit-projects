/*
 * Encoding: UTF-8
 * File:     client.cc
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Client implementation
 *              (see README for more information)
 */

#include "common.h"

const short OBLIGATE_PARAM_COUNT = 6;

/**
 * Prints client program help screen
 */
static void print_help() {
	cout <<	"Name: \n"
		"    client\n"
		"Synopsis:\n"
		"    client -h hostname -p port [-d|u|e|l] file_name\n"
		"Description:\n"
		"    Uploads/downloads/deletes selected file to/from the selected server\n"
		"    or gets file list.\n"
		"    File name must be without absolute or relative path.\n\n"
		"    -h  Server's hostname\n"
		"    -p  Port on which the server is listening (1024-65535)\n"
		"    -d  Downloads selected file from the server\n"
		"    -u  Uploads selected file to the server\n"
		"    -e  Deletes selected file from the server\n"
		"    -l  Prints files that are currently on the server (set file_name\n"
		"        to . for all files or to file_name when interested only\n"
		"        if this particular file is on the server)\n"
		"Example:\n"
		"    client -h eva.fit.vutbr.cz -p 10000 -u test.txt\n"
		"      Uploads file test.txt from the current client's directory\n"
		"      to the actual server's working directory.\n"
		"Author:\n"
		"    Petr Zemek, xzemek02@stud.fit.vutbr.cz, 2007"
		<< endl;
}

/**
 * Prints error message to standard error
 * and program name, if 'print_name' is true
 */
static inline void print_error(const char *err_message, bool print_name = true) {
	if (print_name)
    	cerr << CLIENT_FILE_NAME << ": ";
    cerr << err_message << endl;
}

/**
 * Prints answer from server to the standard output if necessary
 */
static inline void print_answer(const string &answer) {
	if (answer.size() > 0) {
		cout << answer;
		if (answer.at(answer.size() - 1) != '\n')
			cout << endl;
		cout << flush;
	}
}

/**
 * Main
 */
int main(int argc, char **argv) {
	try {
		if (argc != (OBLIGATE_PARAM_COUNT + 1)) {
			print_help();
			return 1;
		}

		Protocol::Request request(argv);

		Client_client client(request.get_hostname(), request.get_port());
		string request_message = Protocol::create_request_message(request);
		client.send_request(request_message);
		Protocol::Answer answer = client.recieve_answer();

		#ifndef NDEBUG
			if (answer.get_data().size() < MAX_DBG_MESSAGE_LENGTH)
				cerr << "Answer: " << answer.get_code() << " "
					<< answer.get_text() << "\n"
					<< answer.get_data() << endl;
		#endif

		string to_output = client.handle_answer(answer);
		print_answer(to_output);
	}
	catch (ParamException &e) {
		print_error(e.what());
		print_error("Try `client --help' for more information.", false);
		return 1;
	}
	catch (BasicException &e) {
		print_error(e.what());
		return 1;
	}
	catch (exception &e) {
		print_error(e.what());
		return 1;
	}

	return 0;
}

/* End of file client.cc */
