/*
 * Encoding: UTF-8
 * File:     Client.cc
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Client implementation
 */

#include "common.h"

////////////////////////////////// Abstract_client /////////////////////////////////////////////

/**
 * Ctor - just initialize basic data
 */
Abstract_client::Abstract_client():
	total_recieved(0), total_sent(0) {
}

/**
 * Ctor - just initialize basic data
 */
Abstract_client::Abstract_client(int client_sock, sockaddr_in info):
	sock(client_sock), sock_info(info),
	total_recieved(0), total_sent(0) {
}

/**
 * Dtor - closes connection to the client/server
 */
Abstract_client::~Abstract_client() {
	close(sock);
}

/**
 * Send message to the client/server
 */
void Abstract_client::send_message(const string &message) {
	#ifndef NDEBUG
		cerr << "Starting sending to : " << inet_ntoa(sock_info.sin_addr) << endl;
	#endif

	int size;
	string to_sent = message;
	Protocol::prepare_message(to_sent);
	if ((size = send(sock, to_sent.c_str(), to_sent.size(), 0)) == -1) {
		throw NetworkException("Message transmission failed.");
	}
	total_sent += size;

	#ifndef NDEBUG
		cerr << "Total sent: " << total_sent << "b" << endl;
	#endif
}

/**
 * Recieve message from the client/server
 */
string Abstract_client::recieve_message() {
	#ifndef NDEBUG
		cerr << "Starting recieving from : " << inet_ntoa(sock_info.sin_addr) << endl;
	#endif

	string message;
	int size;
	while ((size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
		buffer[size] = '\0';
		total_recieved += size;
		message += string(buffer, size);

		#ifndef NDEBUG
			cerr << message.size() << " " << Protocol::get_message_size(message) << endl;
		#endif

		if (message.size() >= Protocol::get_message_size(message))
			break;
	}
	Protocol::remove_message_size(message);

	#ifndef NDEBUG
		if (message.size() < MAX_DBG_MESSAGE_LENGTH)
			cerr << "Recieved message: " << message << endl;
		cerr << "Total recieved: " << total_recieved << "b" << endl;
	#endif

	return message;
}

////////////////////////////////// Client_server /////////////////////////////////////////////

/**
 * Ctor - just initialize client
 * (server should take care about connection data)
 */
Client_server::Client_server(int client_sock, sockaddr_in info):
	Abstract_client(client_sock, info) {
}

/**
 * Recieves request from the client and returns it in a string
 */
string Client_server::recieve_request() {
	return recieve_message();
}

/**
 * Handles request from the client and returns answer
 */
Protocol::Answer Client_server::handle_request(const string &request) {
	Protocol::Request req(request);

	switch (req.get_action()) {
		// Download
		case Protocol::Request::action_download: {
			if (is_directory(req.get_filename())) {
				return Protocol::Answer(Protocol::Answer::ERR_FDIREC,
					req.get_filename() + string(" is a directory"), "");
			}

			if (!is_legal_filename(req.get_filename())) {
				return Protocol::Answer(Protocol::Answer::ERR_FINVAL,
					string("Filename ") + req.get_filename() + string(" is invalid"), "");
			}

			fstream file2send(req.get_filename().c_str(), ios::in);
			if (!file2send) {
				return Protocol::Answer(Protocol::Answer::ERR_NFOUND,
					string("File ")	+ req.get_filename() + string(" wasn't found"), "");
			}

			// Read whole file into string
			string file_data;
			read_file(file2send, file_data);

			return Protocol::Answer(Protocol::Answer::OK_FILE, req.get_filename(), file_data);
		}
		break;

		// Upload
		case Protocol::Request::action_upload: {
			if (is_directory(req.get_filename())) {
				throw RuntimeException(string("Cannot save recieved file (cannot overwrite directory ")
					+ req.get_filename() + ")");
			}

			if (!is_legal_filename(req.get_filename())) {
				return Protocol::Answer(Protocol::Answer::ERR_FINVAL,
					string("Filename ") + req.get_filename() + string(" is invalid"), "");
			}

			fstream recieved_file(req.get_filename().c_str(), ios::out);
			if (!recieved_file) {
				throw RuntimeException("Cannot save recieved file");
			}

			// Save file
			recieved_file << req.get_data();

			return Protocol::Answer(Protocol::Answer::OK_TEXT, req.get_filename(), "");
		}
		break;

		// Delete
		case Protocol::Request::action_delete: {
			if (!is_legal_filename(req.get_filename())) {
				return Protocol::Answer(Protocol::Answer::ERR_FINVAL,
					string("Filename ") + req.get_filename() + string(" is invalid"), "");
			}

			if (Client_server::remove_file(req.get_filename())) {
				return Protocol::Answer(Protocol::Answer::OK_TEXT,
					req.get_filename() + string(" was deleted"), "");
			}
			else {
				return Protocol::Answer(Protocol::Answer::ERR_NFOUND,
					req.get_filename() + string(" cannot be removed/doesn't exists"), "");
			}


		}

		// List
		case Protocol::Request::action_list: {
			if (!is_legal_filename(req.get_filename()) &&
				req.get_filename() != SELECT_ALL_FILES_SYMBOL) {
				return Protocol::Answer(Protocol::Answer::ERR_FINVAL,
					string("Filename ") + req.get_filename() + string(" is invalid"), "");
			}

			return Protocol::Answer(Protocol::Answer::OK_TEXT,
				req.get_filename(),	list_files(req.get_filename()));
		}
		break;
	}

	// This should never happen...
	return Protocol::Answer(Protocol::Answer::ERR_TRANSM, "Unknown error", "");
}

/**
 * Sends answer to the client
 */
void Client_server::send_answer(const Protocol::Answer &answer) {
	send_message(Protocol::answer2string(answer));
}

const char *Client_server::SELECT_ALL_FILES_SYMBOL = ".";

/**
 * Returns list of all files in the server's working directory
 * separated by "\n" (if file_name is ".") or only
 * selected file (if it is in the current server's working directory)
 */
string Client_server::list_files(const string &file_name) {
	string list_of_files;
	dirent **files;
	int count = scandir(".", &files, 0, alphasort);
	if (count > 0) {
		for (int i = 0; i < count; i++) {
			if ((strcmp(files[i]->d_name, ".") == 0) ||
				(strcmp(files[i]->d_name, "..") == 0) ||
				SERVER_FILE_NAME == files[i]->d_name) {
				continue;
			}
			else if (is_directory(files[i]->d_name)) {
				continue;
			}
			else if (file_name == SELECT_ALL_FILES_SYMBOL) {
				list_of_files += string(files[i]->d_name) + "\n";
			}
			else if (file_name == files[i]->d_name) {
				 return string(file_name + "\n");
			}
		}
	}

	return list_of_files;
}

/**
 * Deletes selected file from the server's current
 * working directory and return true if removal was successfull
 * or false otherwise
 */
bool Client_server::remove_file(const string &file_name) {
	return (remove(file_name.c_str()) == 0);
}

////////////////////////////////// Client_client /////////////////////////////////////////////

/**
 * Ctor - connects client to the server
 */
Client_client::Client_client(const string &client_hostname, int client_port):
	hostname(client_hostname), port(client_port) {

	// Get server's info
	if (0 == (host = gethostbyname(hostname.c_str()))) {
		throw NetworkException("Host information retrieval failed");
	}

	// Create socket
	if (-1 == (sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
		throw NetworkException("Socket creation failed");
	}

	// Fill structure sockaddr_in
	sock_info.sin_family = PF_INET; // Protocols family
	sock_info.sin_port = htons(port); // Port number
	memcpy(&(sock_info.sin_addr), host->h_addr, host->h_length); // Selected server IP address

	// Establish connection
	// (reinterpret_cast<> is necessary here...)
	if (-1 == connect(sock, reinterpret_cast<sockaddr *>(&sock_info), sizeof(sock_info))) {
		throw NetworkException("Connection to the selected server failed");
	}
}

/**
 * Sends request to the server
 * (before this function must create_request_message() be called)
 */
void Client_client::send_request(const string &request) {
	send_message(request);
}

/**
 * Recieves answer from the server
 */
Protocol::Answer Client_client::recieve_answer() {
	return Protocol::string2answer(recieve_message());
}

/**
 * Handles answer from the server and (if needed) returns answer to be outputted
 */
string Client_client::handle_answer(const Protocol::Answer &answer) {
	string code = answer.get_code();
	if (code == Protocol::Answer::OK_FILE) {
		fstream recieved_file(answer.get_text().c_str(), ios::out);
		if (!recieved_file) {
			throw RuntimeException("Cannot save recieved file");
		}

		// Save file
		recieved_file << answer.get_data();
	}
	else if (code == Protocol::Answer::OK_TEXT) {
		return answer.get_data();
	}
	else {
		throw RuntimeException(answer.get_text());
	}

	return string("");
}

/* End of file Client.cc */
