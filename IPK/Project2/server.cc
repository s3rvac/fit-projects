/*
 * Encoding: UTF-8
 * File:     server.cc
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Server implementation
 *              (see README for more information)
 */

#include "common.h"
#include "Server.h"

const int OBLIGATE_PARAM_COUNT = 1;
const short PORT_PARAM_IDX = 1;

/**
 * Prints client program help screen
 */
static void print_help() {
	cout <<	"Name: \n"
		"    server\n"
		"Synopsis:\n"
		"    server port\n"
		"Description:\n"
		"    Concurrent file server from which are clients available\n"
		"    to download and/or upload files.\n\n"
		"    port  Port on which the server will be listening (1024-65535)\n"
		"Example:\n"
		"    server 10000\n"
		"      Starts server listening on the port 10000.\n"
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
    	cerr << SERVER_FILE_NAME << ": ";
    cerr << err_message << endl;
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

		// Check port
		int port = atoi(argv[PORT_PARAM_IDX]);
		if (!is_legal_port(port)) {
			throw ParamException(string("Invalid port: ") +
				string(argv[PORT_PARAM_IDX]));
		}

		// Create server
		Server server(port);

		for (;;) {
			try {
				Client_server client = server.accept_client();
				pid_t pid = fork();
				if (pid < 0) {
					throw ProcessException("Creation of a new server process failed (fork)");
				}
				else if (pid == 0) {
					// Child process, serve the accepted client
					string request = client.recieve_request();

					#ifndef NDEBUG
						if (request.size() < MAX_DBG_MESSAGE_LENGTH)
							cerr << "Request: " << request << endl;
					#endif

					try {
						Protocol::Answer answer = client.handle_request(request);
						client.send_answer(answer);
					}
					catch (BasicException &e) {
						client.send_answer(Protocol::Answer(
							Protocol::Answer::ERR_SERVER,
							string("Server error: ") + e.what(),
							""));
						throw;
					}

					return 0;
				}
			}
			catch (BasicException &e) {
				print_error(e.what());
			}
		}
	}
	catch (ParamException &e) {
		print_error(e.what());
		print_error("Try `server --help' for more information.", false);
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

/* End of file server.cc */
