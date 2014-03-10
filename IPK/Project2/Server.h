/*
 * Encoding: UTF-8
 * File:     Server.h
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Server class
 *
 */

#ifndef __SERVER_H__
	#define __SERVER_H__

#include "Client.h" /* Client class */

/**
 * Server
 */
class Server {
public:
	/**
	 * Ctor - starts server listening on the selected port
	 */
	Server(int port);

	/**
	 * Dtor - closes listening and restores
	 * default signal handlers/block masks
	 */
	~Server();

	/**
	 * Accept connection to the client
	 */
	Client_server accept_client();

private:
	/**
	 * Sets signal blocking, masks etc. for the server
	 */
	void set_signal_handling();

	/**
	 * Restores signal blocking, masks etc. for the server
	 */
	void restore_signal_handling();

	// Blocking masks
	sigset_t block_current;
	sigset_t block_old; // (for restoration)
	// Handlers
	struct sigaction sigchld_current;
	struct sigaction sigchld_old; // (for restoration)

	enum {BUFFER_SIZE = 4096};
	static const int MAX_CONNECTIONS_COUNT = 20;

	int sock;
	sockaddr_in serv_info;
	socklen_t serv_info_size;
	int port;
};

#endif /* #ifndef __SERVER_H__ */

/* End of file Server.h */
