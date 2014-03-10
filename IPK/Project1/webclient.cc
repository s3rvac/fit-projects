/*
 * Encoding: UTF-8
 * File:     webclient.cc
 * Project:  Webclient
 * Date:     16.2.2007
 * Author:   Petr Zemek, xzemek02@stud.fit.vutbr.cz
 * Description: (only in CZ)
 *
 * Vytvoreni klientske sitove aplikace v prostredi UNIXu s vyuzitim
 * komunikacniho rozhrani BSD sockets.
 *
 * Vytvorte program (klient) s vyuzitim rozhrani schranek (BSD sockets),
 * ktery implementuje stahovani zadaneho objektu pomoci URL z WWW serveru
 * s vyuzitim HTTP protokolu do souboru ulozeneho v lokalnim souborovem systemu.
 * Vytvorte program v jazyce C/C++, ktery je  prelozitelny na studentskem
 * unixovem serveru eva vcetne funkcniho Makefile souboru (program prelozitelny
 * po zadani prikazu make) . Program vyuziva spojovanou sluzbu (protokol TCP).
 * Jmeno prelozeneho programu klienta bude webclient. Program predpoklada jeden
 * povinny parametr a to URL identifikujici objekt, ktery bude ulozen do lokaniho
 * souboroveho systemu do aktualniho adresare. Pokud v dotazu URL neni uvedeno
 * jmeno souboru, obsah bude ulozen do souboru index.html . Program musi
 * podporovat stavove kody HTTP protokolu pro presmerovani pozadavku 3xx.
 * Oznameni o chybach, ktere mohou nastat, bude vytisteno na standardni
 * chybovy vystup (stderr).
 *
 * Synopsis: webclient URL
 *
 * Priklad pouziti:
 * webclient http://www.fit.vutbr.cz
 *   stahne defaultni stranku a ulozi do souboru index.html
 * webclient http://www.fit.vutbr.cz:80/common/img/fit_logo_cz.gif
 *  stahne a ulozi obrazek fit_logo_cz.gif do souboru
 */

#include <iostream>
#include <fstream>
#include <algorithm>
#include <iterator>
#include <exception>
#include <stdexcept>
#include <string>
#include <cctype>
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

/*
 * Debugging
 */
#ifndef NDEBUG
	#define NDEBUG
#endif
#include <cassert>

/*
 * Used http protocol (for HTTP 1.0 undef HTTP11)
 */
#ifndef HTTP11
	#define HTTP11
#endif

/*
 * Uncomment this for redirection limitation
 */
/*
#ifndef LIMIT_REDIRECT
 	#define LIMIT_REDIRECT
 	const int MAX_REDIRECTIONS_COUNT = 40;
#endif
*/

using std::min;
using std::cout;
using std::cerr;
using std::endl;
using std::string;
using std::ofstream;
using std::ostream_iterator;
using std::exception;
using std::bad_alloc;
using std::length_error;

const int BUFFER_SIZE = 4096;
const int DEFAULT_PORT = 80;
const char *DEFAULT_FILE_NAME = "index.html";
const int HTTP_CODE_DIGITS = 3; // Number of digits in the http state code
const int CHUNK_SIZE_BASE = 16; // Chunks data base (hexa)

/*
 * Structure containing info about selected url
 */
struct url_info {
	int port;
	string server_name;
	string object_name;
};

/**
 * Case-insensitive comparation
 */
bool ci_equal(char ch1, char ch2) {
	return toupper(static_cast<unsigned char>(ch1)) ==
		toupper(static_cast<unsigned char>(ch2));
}

/**
 * Case-insensitive version of std::string.find()
 */
string::size_type ci_find(const string &str1, const string &str2) {
	string::const_iterator pos = search(str1.begin(), str1.end(),
		str2.begin(), str2.end(), ci_equal);

	return (pos == str1.end()) ? string::npos : (pos - str1.begin());
}

/**
 * Cuts off protocol name, saves port (if specified), separate filename
 * (if specified) and returns it in url_info structure.
 */
url_info transform_url(const char *selected_url) {
	url_info info;
	string url(selected_url);

	// Cut off protocol name
	string::size_type protocol_index = ci_find(url, string("http://"));
	if (protocol_index != string::npos) {
		url.erase(protocol_index, protocol_index + 7);
	}

	// Get port name (if selected)
	// like: www.fit.vutbr.cz:80/common/img/fit_logo_cz.gif
	// -> should get 80
	string::size_type first_colon = url.find_first_of(":");
	string::size_type first_slash = url.find_first_of("/");
	if (first_colon < (first_slash - 1)) {
		string num(url, first_colon + 1, url.size() - first_slash - 1);
		info.port = atoi(num.c_str());
		if (info.port <= 0)
			info.port = DEFAULT_PORT;

		// Remove port number from url
		url.erase(first_colon, first_slash - first_colon);
	}
	else {
		if (first_colon != string::npos) {
			// Just remove the '/' after domain name
			url.erase(first_slash, 1);
		}
		info.port = DEFAULT_PORT;
	}

	// Save the rest of it into the object_name
	first_slash = url.find_first_of("/");
	if (first_slash < string::npos)
		info.object_name = url.substr(first_slash + 1);
	else
		info.object_name = "";

	// Save server name
	info.server_name = url.substr(0, first_slash);

	#ifndef NDEBUG
		cerr << info.server_name << "\t"
			<< info.port << "\t"
			<< info.object_name << endl;
	#endif

	return info;
}

/**
 * Returns state code from http headers (or 0, if an error occurs)
 */
inline int get_state_code(const string &headers) {
	return atoi(string(headers, headers.find_first_of(" ") + 1,
		HTTP_CODE_DIGITS).c_str());
}

/**
 * Returns whether data are chunked or not
 */
inline bool get_is_chunked(const string &headers) {
	return ci_find(headers, string("Transfer-Encoding: chunked"))
		!= string::npos;
}

/**
 * Returns url from http headers (Location: url)
 * or empty string, if an error occurs
 */
inline string get_location(const string &headers) {
	string::size_type location_index = ci_find(headers, string("\r\nLocation: "));
	if (location_index == string::npos)
		return string("");

	string url;
	for (string::size_type i = location_index + strlen("\r\nLocation: ");
		i < headers.size() && !isspace(headers[i]); i++) {
		url += headers[i];
	}

	return url;
}

/**
 * Get current chunk size
 */
size_t get_chunk_size(const string &data) {
	if (data.find("\r\n") == string::npos)
		return 0;

	string::size_type semicolon_location = data.find(";");
	string::size_type new_line_location = data.find("\r\n");
	if (semicolon_location == string::npos &&
		new_line_location == string::npos)
			return 0;

	string size(data, 0, min(semicolon_location, new_line_location));
	return strtoul(size.c_str(), 0, CHUNK_SIZE_BASE);
}

/**
 * Prints program help screen
 */
void print_help() {
	cout <<	"Name: \n"
		"    webclient - the non-interactive network downloader\n"
		"Synopsis:\n"
		"    webclient URL\n"
		"Description:\n"
		"    Program downloads an object from the selected WWW server\n"
		"    using HTTP protocol to a file in the local file system.\n"
		"    Returns 0 if everything went ok and > 0 if there was an error.\n"
		"Examples:\n"
		"    webclient http://www.fit.vutbr.cz\n"
		"      downloads default page and saves it to a file named index.html\n"
		"    webclient http://www.fit.vutbr.cz:80/common/img/fit_logo_cz.gif\n"
		"      downloads and saves image fit_logo_cz.gif to a file\n"
		"Author:\n"
		"    Petr Zemek, xzemek02@stud.fit.vutbr.cz, 2007"
		<< endl;
}

/**
 * Prints error message to standard error
 */
inline void print_error(const char *err_message) {
    cerr << "webclient: " << err_message << endl;
}

/**
 * Main
 */
int main(int argc, char **argv) {
	if (argc != 2) {
		print_help();
		return 1;
	}

try {
	string url(argv[1]);
	if (ci_find(url, string("http://")) == string::npos) {
		print_error("URL must beginning with 'http://'.");
		return 1;
	}

	#ifdef LIMIT_REDIRECT
		int redirections_count = 0;
	#endif

	bool finished = false;       // Status indicator
	do {
		int size = 0;            // Number of sent/recieved bytes
		int sock;                // Socket
		hostent *host;           // Selected server info
		sockaddr_in server_sock; // Selected server's socket

		// Cut selected url into separate parts
		url_info info = transform_url(url.c_str());
		if (info.server_name.empty()) {
			// Reload original server name
			url_info info2 = transform_url(string(argv[1]).c_str());
			info.server_name = info2.server_name;
		}

		// Get info about selected server
		if (0 == (host = gethostbyname(info.server_name.c_str()))) {
			print_error("Host information retrieval failed.");
			return 1;
		}

		// Create socket
		if (-1 == (sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))) {
			print_error("Socket creation failed.");
			return 1;
		}

		// Fill structure sockaddr_in
		server_sock.sin_family = AF_INET; // Protocols family
		server_sock.sin_port = htons(info.port); // Port number
		memcpy(&(server_sock.sin_addr),
			host->h_addr, host->h_length); // Selected server IP address

		// Establish connection
		if (-1 == connect(sock, (sockaddr *)&server_sock, sizeof(server_sock))) {
			print_error("Connection to the selected server failed.");
			return 1;
		}


		#ifndef NDEBUG
			cerr << "Starting sending:" << endl;
		#endif
		// Set http headers/commands
		#ifdef HTTP11
 			string cmd = "GET /" + info.object_name + " HTTP/1.1\r\n"
  				"Host: " + info.server_name + "\r\n"
  				"Connection: close\r\n\r\n";
  		#else
 			string cmd = "GET /" + info.object_name + " HTTP/1.0\r\n"
 				"Host: " + info.server_name + "\r\n\r\n";
 		#endif

 		#ifndef NDEBUG
			cerr << cmd << endl;
		#endif
		// Send data
		if (-1 == (size = send(sock, cmd.c_str(), cmd.size(), 0))) {
			print_error("Data sending failed.");
			return 1;
    	}
    	#ifndef NDEBUG
    		cerr << "Sent: " << size << "b" << endl;
		#endif

		#ifndef NDEBUG
			cerr << "Starting recieving:" << endl;
		#endif
		// Recieve data
		char buffer[BUFFER_SIZE] = {'\0'}; // Recieving buffer
		string output_data = "";
		string headers = "";
		bool got_headers = false;
		int current_chunk_size = 0;
		int total_size = 0;
		int recieved_size = 0;
		while ((size = recv(sock, buffer, BUFFER_SIZE - 1, 0)) > 0) {
			buffer[size] = '\0';
			recieved_size += size;

			// Append buffer to output_date
			// Conversion to string is needed because of possible NULL bytes
			output_data += string(buffer, size);

			// Save headers
			if (!got_headers) {
				string::size_type offset = output_data.find("\r\n\r\n");
				if (offset != string::npos) {
					headers = string(output_data.begin(), output_data.begin() + offset);
					got_headers = true;

					#ifndef NDEBUG
						cerr << "Headers:" << endl << headers << endl << endl;
					#endif

					// Get rid of headers from output data
					output_data.erase(output_data.begin(), output_data.begin() + offset + 4);
				}
			}

			// Manage chunks
			if (got_headers && get_is_chunked(headers)) {
				if (current_chunk_size <= 0) {
					// Remove \r\n before chunk size
 					if (total_size > 0) {
 						output_data.erase(total_size, 2);
 					}

					// Get current chunk size
 					string chunks_starting = output_data.substr(total_size);
					current_chunk_size = get_chunk_size(chunks_starting);

 					// Remove line with chunk size information
					output_data.erase(total_size, chunks_starting.find("\r\n") + 2);

					total_size += current_chunk_size;

					#ifndef NDEBUG
						cerr << "Chunk size: " << current_chunk_size << endl;
						cerr << "Total size: " << total_size << endl;
						cerr << "Data size: " << output_data.size() << endl;
					#endif
				}

				current_chunk_size -= size;
			}
		}

		#ifndef NDEBUG
			cerr << "Recieved: " << recieved_size << "b" << endl;
		#endif

		// Close connection
   		close(sock);

		// Check server's return code (http)
		int state_code = get_state_code(headers);
		switch (state_code) {
			// 1xx - informational message
			case 100: case 101: {
				continue;
			}
			// 2xx - success
			case 200: case 201:
			case 202: case 203:
			case 204: case 205:
			case 206: {
				// We need do remove the trailing \r\n0\r\n\r\r
				if (got_headers && get_is_chunked(headers)) {
					output_data.erase(output_data.size() - strlen("\r\n0\r\n\r\r"));
				}

				// "Generate" filename according to the selected url
				string filename;

				// Remove GET parameters
				string::size_type last_interr = info.object_name.find_first_of("?");
				if (last_interr != string::npos) {
					info.object_name.erase(last_interr);
				}

				string::size_type last_slash = info.object_name.find_last_of("/");
				if (last_slash == string::npos) {
					filename = info.object_name;
				}
				else {
					filename = info.object_name.substr(last_slash + 1);
				}
				if (filename.empty()) {
					filename = DEFAULT_FILE_NAME;
				}

				// Write data into it
				ofstream output_file(filename.c_str());
				copy(output_data.begin(), output_data.end(),
					ostream_iterator<char>(output_file,""));

				finished = true;
				}
   				break;

			// 3xx - redirect to another url
			case 300: case 301:
			case 302: case 303:
			case 304: case 305:
			case 307: {
				#ifdef LIMIT_REDIRECT
					redirections_count++;
					if (redirections_count > MAX_REDIRECTIONS_COUNT) {
						print_error("Maximum redirections count limit reached.");
						return 1;
					}
				#endif

				// Get new url and do one more pass
				url = get_location(headers);
				if (url.size() == 0) {
					print_error("Cannot get new location from http headers.");
					return 1;
				}
				}
				break;

			// 4xx - error on the client's part
			case 400: case 401:	case 402:
			case 403: case 404: case 405:
			case 406: case 407: case 408:
			case 409: case 410: case 411:
			case 412: case 413: case 414:
			case 415: case 416: case 417:
				print_error(headers.c_str());
				return 1;
				break;

			// 5xx - error on the server's part
			case 500: case 501:
			case 502: case 503:
			case 504: case 505:
				print_error(headers.c_str());
				return 1;
				break;

			default:
				print_error("Unsupported or illegal http state code returned.");
				return 1;
		}

	} while (!finished);

}
catch (bad_alloc &e) {
	print_error("Memory innsufficiency.");
	return 1;
}
catch (length_error &e) {
	print_error("Selected index is out of limit.");
	return 1;
}
catch (exception &e) {
	print_error(e.what());
	return 1;
}

	return 0;
}

/* End of file webclient.cc */
