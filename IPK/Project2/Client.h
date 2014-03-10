/*
 * Encoding: UTF-8
 * File:     Client.h
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Client header
 *
 */

#ifndef __CLIENT_H__
	#define __CLIENT_H__

/**
 * Abstract client (base class)
 */
class Abstract_client {
public:
	/**
	 * Ctors - just initialize basic data
	 */
	Abstract_client();
	Abstract_client(int client_sock, sockaddr_in info);

	/**
	 * Dtor - closes connection to the client/server
	 */
	virtual ~Abstract_client() =0;

protected:
	/**
	 * Recieve message from the client/server
	 */
	string recieve_message();

	/**
	 * Send message to the client/server
	 */
	void send_message(const string &message);

	enum {BUFFER_SIZE = 4096};
	char buffer[BUFFER_SIZE];
	int sock;
	sockaddr_in sock_info;
	int total_recieved;
	int total_sent;
};

/**
 * Client_server (connection from server to the client)
 */
class Client_server: public Abstract_client {
public:
	/**
	 * Ctor - just initialize client
	 * (server should take care about connection data)
	 */
	Client_server(int client_sock, sockaddr_in info);

	/**
	 * Recieves request from the client and returns it in a string
	 */
	string recieve_request();

	/**
	 * Handles request from the client and returns answer
	 */
	Protocol::Answer handle_request(const string &request);

	/**
	 * Sends answer to the client
	 */
	void send_answer(const Protocol::Answer &answer);

private:
	static const char *SELECT_ALL_FILES_SYMBOL;

	/**
	 * Returns list of all files in the server's working directory
	 * separated by "\n" (if file_name is ".") or only
	 * selected file (if it is in the current server's working directory)
	 */
	string list_files(const string &file_name);

	/**
	 * Deletes selected file from the server's current
	 * working directory and return true if removal was successfull
	 * or false otherwise
	 */
	 bool remove_file(const string &file_name);
};

/**
 * Client_client (client side)
 */
class Client_client: public Abstract_client {
public:
	Client_client(const string &client_hostname, int client_port);

	/**
	 * Sends request to the server
	 * (before this function create_request_message() must be called)
	 */
	void send_request(const string &request);

	/**
	 * Recieves answer from the server
	 */
	Protocol::Answer recieve_answer();

	/**
	 * Handles answer from the server and (if needed) returns answer
	 * to be outputted
	 */
	string handle_answer(const Protocol::Answer &answer);

private:
	string hostname;
	int port;
	hostent *host; // Server info
};

#endif /* #ifndef __CLIENT_H__ */

/* End of file Client.h */
