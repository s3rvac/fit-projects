/*
 * Encoding: UTF-8
 * File:     Protocol.cc
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Protocol implementation
 */

#include "common.h"

////////////////////////////////// Request /////////////////////////////////////////////

const short Protocol::Request::HOST_PARAM_IDX;
const short Protocol::Request::PORT_PARAM_IDX;
const short Protocol::Request::ACTION_PARAM_IDX;
const short Protocol::Request::FILE_PARAM_IDX;

const string Protocol::Request::ACT_GET = "GET";
const string Protocol::Request::ACT_PUT = "PUT";
const string Protocol::Request::ACT_DEL = "DEL";
const string Protocol::Request::ACT_LST = "LST";

/**
 * Ctor - checks client program parameters and save them
 */
Protocol::Request::Request(char **argv): data("") {
	// Check hostname
	if (strcmp(argv[HOST_PARAM_IDX - 1], "-h") == 0) {
		hostname = string(argv[HOST_PARAM_IDX]);
	}
	else {
		throw ParamException(string("Invalid option: ") +
			string(argv[HOST_PARAM_IDX - 1]));
	}

	// Check port
	if (strcmp(argv[PORT_PARAM_IDX - 1], "-p") == 0) {
		port = atoi(argv[PORT_PARAM_IDX]);
		if (!is_legal_port(port)) {
			throw ParamException(string("Invalid port: ") +
				string(argv[PORT_PARAM_IDX]));
		}
	}
	else {
		throw ParamException(string("Invalid option: ") +
			string(argv[PORT_PARAM_IDX - 1]));
	}

	// Check action
	if (strcmp(argv[ACTION_PARAM_IDX], "-d") == 0) {
		action = action_download;
	}
	else if (strcmp(argv[ACTION_PARAM_IDX], "-u") == 0) {
		action = action_upload;
	}
	else if (strcmp(argv[ACTION_PARAM_IDX], "-e") == 0) {
		action = action_delete;
	}
	else if (strcmp(argv[ACTION_PARAM_IDX], "-l") == 0) {
		action = action_list;
	}
	else {
		throw ParamException(string("Invalid action: ") +
			string(argv[ACTION_PARAM_IDX]));
	}

	// Check filename
	filename = string(argv[FILE_PARAM_IDX]);
}

/**
 * Ctor - transform request message into Request
 */
Protocol::Request::Request(const string &request) {
	string req(request);
	string::size_type first_space = req.find(" ");
	if (first_space == string::npos) {
		throw RuntimeException("Request message has wrong format (size)");
	}

	// Action
	string request_action(req, 0, first_space);
	if (request_action == Protocol::Request::ACT_GET) {
		action = action_download;
	}
	else if (request_action == Protocol::Request::ACT_PUT) {
		action = action_upload;
	}
	else if (request_action == Protocol::Request::ACT_DEL) {
		action = action_delete;
	}
	else if (request_action == Protocol::Request::ACT_LST) {
		action = action_list;
	}
	else {
		throw RuntimeException("Request message has wrong format (action)");
	}
	req.erase(0, first_space + strlen(" "));

	string::size_type first_end_marker = req.find(Protocol::END_MARKER);
	if (first_end_marker == string::npos) {
		throw RuntimeException("Request message has wrong format (filename)");
	}

	// Filename
	filename = string(req, 0, first_end_marker);
	req.erase(0, first_end_marker + Protocol::END_MARKER.size());

	// Data
	data = req;
}

/**
 * Returns text representation of selected action
 * (see README for protocol details)
 */
string Protocol::Request::get_action_code() const {
	if (action == action_download) {
		return string(Protocol::Request::ACT_GET);
	}
	else if (action == action_upload) {
		return string(Protocol::Request::ACT_PUT);
	}
	else if (action == action_delete) {
		return string(Protocol::Request::ACT_DEL);
	}
	else if (action == action_list) {
		return string(Protocol::Request::ACT_LST);
	}
	else {
		throw RuntimeException("Invalid action in the request");
	}

	return string("");
}

////////////////////////////////// Answer /////////////////////////////////////////////

const string Protocol::Answer::ERR_NFOUND = "100";
const string Protocol::Answer::ERR_FINVAL = "101";
const string Protocol::Answer::ERR_FDIREC = "102";
const string Protocol::Answer::ERR_TRANSM = "103";
const string Protocol::Answer::ERR_SERVER = "104";
const string Protocol::Answer::OK_FILE = "200";
const string Protocol::Answer::OK_TEXT = "201";

/**
 * Ctor - just do initialization from the server's answer message
 */
Protocol::Answer::Answer(const string &answer_code,
	const string &answer_text, const string &answer_data):
	code(answer_code), text(answer_text), data(answer_data) {
}

/**
 * Dtor
 */
Protocol::Answer::~Answer() {}


////////////////////////////////// Protocol /////////////////////////////////////////////

const int Protocol::MESSAGE_SIZE_BASE;
const string Protocol::END_MARKER = "\r\n";

/**
 * Creates request message from the selected request
 */
string Protocol::create_request_message(const Protocol::Request &request) {
	switch (request.get_action()) {
		// Upload - we have to add file int request
		case Protocol::Request::action_upload: {
			// Check whether selected "file" isn't directory
			if (is_directory(request.get_filename())) {
				throw RuntimeException("Cannot upload directories");
			}

			fstream file2send(request.get_filename().c_str(), ios::in);
			if (!file2send) {
				throw RuntimeException(string("Error opening ")
					+ request.get_filename());
			}

			// Read whole file into string
			string file_data;
			read_file(file2send, file_data);

			return string(request.get_action_code() + " "
				+ get_only_filename(request.get_filename())
				+ Protocol::END_MARKER
				+ file_data);
		}
		break;

		default:
		break;
	}

	return string(request.get_action_code() + " "
		+ request.get_filename()
		+ Protocol::END_MARKER);
}

/**
 * Prepare message to be sent
 */
void Protocol::prepare_message(string &message) {
	// Put size info in the beginning of the message
	// Sorry about this crazy way ;f
	stringstream s;
	// 2 * END_MARKER for size line and request line
	s << (message.size() + 2 * Protocol::END_MARKER.size());
	string size_info;
	s >> size_info;
	message.insert(0, string(size_info) + string(Protocol::END_MARKER));
}

/**
 * Returns message size (0 if error)
 */
size_t Protocol::get_message_size(const string &message) {
	string::size_type end_marker = message.find(Protocol::END_MARKER);
	if (end_marker != string::npos) {
		return strtol(string(message, 0, end_marker).c_str(), 0, Protocol::MESSAGE_SIZE_BASE);
	}

	return 0;
}

/**
 * Transforms string answer into Answer class
 */
Protocol::Answer Protocol::string2answer(const string &answer_message) {
	string answer(answer_message);
	string::size_type first_space = answer.find(" ");
	if (first_space == string::npos) {
		throw RuntimeException("Answer message has wrong format (code)");
	}

	string answer_code(answer, 0, first_space);
	answer.erase(0, first_space + strlen(" "));

	string::size_type first_end_marker = answer.find(Protocol::END_MARKER);
	if (first_end_marker == string::npos) {
		throw RuntimeException("Answer message has wrong format (text)");
	}

	string answer_text(answer, 0, first_end_marker);
	answer.erase(0, first_end_marker + Protocol::END_MARKER.size());

	return Protocol::Answer(answer_code, answer_text, answer);
}

/**
 * Transforms Answer into string
 */
string Protocol::answer2string(const Answer &answer) {
	return string(answer.get_code() + " "
		+ answer.get_text()
		+ Protocol::END_MARKER
		+ answer.get_data());
}

/**
 * Removes message size from the message
 */
void Protocol::remove_message_size(string &message) {
	string::size_type end_marker = message.find(Protocol::END_MARKER);
	if (end_marker != string::npos) {
		message.erase(0, end_marker + Protocol::END_MARKER.size());
	}
}

/**
 * Removes request info from the message
 */
void Protocol::remove_request_info(string &message) {
	string::size_type end_marker = message.find(Protocol::END_MARKER);
	if (end_marker != string::npos) {
		message.erase(0, end_marker + Protocol::END_MARKER.size());
	}
}

/**
 * Removes protocol information from the message
 */
void Protocol::remove_protocol_info(string &message) {
	Protocol::remove_message_size(message);
	Protocol::remove_request_info(message);
}

/* End of file Protocol.cc */
