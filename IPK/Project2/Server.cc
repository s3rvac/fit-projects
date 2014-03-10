/*
 * Encoding: UTF-8
 * File:     Server.cc
 * Project:  Client/server file transfer
 * Date:     1.3.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: Server class - implementation
 */

#include "common.h"

const int Server::MAX_CONNECTIONS_COUNT;

/**
 * Ctor - starts server listening on the selected port
 */
Server::Server(int listen_port): port(listen_port) {
	// Create socket
	if (-1 == (sock = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP))) {
		throw NetworkException("Socket creation failed");
	}

	serv_info.sin_family = PF_INET; // Protocols family
	serv_info.sin_port = htons(port); // Port number
	serv_info.sin_addr.s_addr  = htonl(INADDR_ANY); // Set IP address to any interface
	serv_info_size = sizeof(serv_info);

	// Set SO_REUSEADDR on the socket to true (1)
	int optval = 1;
	if (-1 == setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval))) {
		throw NetworkException("Adding socket re-use address option failed");
	}

	// Bind the "name" (address) to the socket
	// reinterpret_cast<> is necessary here...
	if (-1 == bind(sock, reinterpret_cast<sockaddr *>(&serv_info), serv_info_size)) {
		close(sock);
		throw NetworkException("Name bind to the socket failed");
	}

	// Create a client queue
	if (-1 == listen(sock, MAX_CONNECTIONS_COUNT)) {
		close(sock);
		throw NetworkException("Queue creation failed");
	}

	set_signal_handling();
}

/**
 * Dtor - closes listening and restores
 * default signal handlers/block masks
 */
Server::~Server() {
	close(sock);
	restore_signal_handling();
}

/**
 * Accept connection to the client
 */
Client_server Server::accept_client() {
	sockaddr_in client_info;
	socklen_t client_info_size = sizeof(client_info);
	// reinterpret_cast<> is necessary here...
	int client_sock = accept(sock, reinterpret_cast<sockaddr *>(&client_info), &client_info_size);
	if (-1 == client_sock) {
		throw NetworkException(string("Client acceptance failed (") + inet_ntoa(client_info.sin_addr) + ")");
	}

	#ifndef NDEBUG
		cerr << "Established connection from: " << inet_ntoa(client_info.sin_addr) << endl;
	#endif

	return Client_server(client_sock, client_info);
}

/**
 * Sets signal blocking, masks etc. for the server
 */
void Server::set_signal_handling() {
	// Set up blocking mask for all processes
	sigfillset(&block_current);
	sigdelset(&block_current, SIGTERM);
	sigdelset(&block_current, SIGCHLD);

	// Establish signal handler for SIGCHLD - to avoid zombie processes
	sigaction(SIGCHLD, 0, &sigchld_old);
	sigemptyset(&sigchld_current.sa_mask);
	sigchld_current.sa_handler = SIG_IGN;
	sigchld_current.sa_flags = 0;
	sigaction(SIGCHLD, &sigchld_current, 0);

	#ifndef NDEBUG
		sigdelset(&block_current, SIGINT);
	#endif

	sigprocmask(SIG_BLOCK, &block_current, &block_old);
}

/**
 * Restores signal blocking, masks etc. for the server
 */
void Server::restore_signal_handling() {
	sigprocmask(SIG_BLOCK, &block_old, 0);
	sigaction(SIGCHLD, &sigchld_old, 0);
}

/* End of file Server.cc */
