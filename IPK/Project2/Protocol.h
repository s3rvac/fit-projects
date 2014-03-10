/*
 * Encoding: UTF-8
 * File:     Protocol.h
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Protocol header
 *
 */

#ifndef __PROTOCOL_H__
	#define __PROTOCOL_H__

#include <string>

using std::string;

/**
 * Protocol
 */
class Protocol {
public:
	/**
 	 * Request
 	 */
	class Request {
	public:
		// Possible actions
		enum actions {
			action_download,
			action_upload,
			action_delete,
			action_list
		};

		// Commands (in request) according to the actions
		static const string ACT_GET;
		static const string ACT_PUT;
		static const string ACT_DEL;
		static const string ACT_LST;

		/**
 		 * Ctor - checks client program parameters and save them
 		 */
		Request(char **argv);

		/**
 		 * Ctor - transform request message into Request
 		 */
		Request(const string &request);

		/**
		 * Access methods
		 */
		string get_hostname() const { return hostname; }
		string get_filename() const { return filename; }
		string get_data() const { return data; }
		actions get_action() const { return action; }
		int get_port() const { return port; }

		/**
 		 * Returns text representation of selected action
 		 * (see README for protocol details)
 		 */
		string get_action_code() const;

	private:
		static const short HOST_PARAM_IDX = 2;
		static const short PORT_PARAM_IDX = 4;
		static const short ACTION_PARAM_IDX = 5;
		static const short FILE_PARAM_IDX = 6;

		string hostname;
		int port;
		actions action;
		string filename;
		string data;
	};

public:
	/**
	 * Answer
	 */
	class Answer {
	public:
		// Possible answer codes (see README for more info)
		static const string ERR_NFOUND;
		static const string ERR_FINVAL;
		static const string ERR_FDIREC;
		static const string ERR_TRANSM;
		static const string ERR_SERVER;
		static const string OK_FILE;
		static const string OK_TEXT;

		/**
		 * Ctor - just do initialization from the server's answer message
		 */
		Answer(const string &answer_code,
			const string &answer_text,
			const string &answer_data);

		/**
		 * Dtor
		 */
		~Answer();

		/**
		 * Access methods
		 */
		string get_code() const { return code; }
		string get_text() const { return text; }
		string get_data() const { return data; }

	private:
		string code;
		string text;
		string data;
	};

public:
	/**
	 * Creates request message from the selected request
	 */
	static string create_request_message(const Request &request);

	/**
	 * Prepare message to be sent
	 */
	static void prepare_message(string &message);

	/**
	 * Returns message size
	 */
	static size_t get_message_size(const string &message);

	/**
	 * Transforms string answer into Answer class
	 */
	static Answer string2answer(const string &answer_message);

	/**
	 * Transforms Answer into string
	 */
	static string answer2string(const Answer &answer);

	/**
	 * Removes message size from the message
	 */
	static void remove_message_size(string &message);

	/**
 	 * Removes request info from the message
 	 */
	static void remove_request_info(string &message);

	/**
	 * Removes protocol information from the message
	 */
	static void remove_protocol_info(string &message);

private:
	Protocol(){}
	~Protocol(){}

	static const int MESSAGE_SIZE_BASE = 10;
	static const string END_MARKER;
};

#endif /* #ifndef __PROTOCOL_H__ */

/* End of file Protocol.h */
