#include "HTTP_Server.h"
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>

// pulled these from https://www.w3.org/Protocols/HTTP/HTRESP.html
const char _status_code_text[8][50] = {
	"HTTP/1.1 200 OK\r\n\r\n",
	"HTTP/1.1 201 CREATED\r\n\r\n",
	"HTTP/1.1 400 Bad request\r\n\r\n",
	"HTTP/1.1 401 Unauthorized\r\n\r\n",
	"HTTP/1.1 403 Forbidden\r\n\r\n",
	"HTTP/1.1 401 Not found\r\n\r\n",
	"HTTP/1.1 500 Internal Error\r\n\r\n",
	"HTTP/1.1 501 Not implemented\r\n\r\n"
};

void init_server(HTTP_Server * http_server, int port) {
	http_server->port = port;

	int server_socket = socket(AF_INET, SOCK_STREAM, 0);

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(server_socket, (struct sockaddr *) &server_address, sizeof(server_address));

	listen(server_socket, 5);

	http_server->socket = server_socket;
	printf("HTTP Server Initialized\nPort: %d\n", http_server->port);
}

void http_set_status_code(
		HTTP_Server* http_server,
		const enum http_status_code_e status_code)
{
	memset(http_server->status_code, 0, HTTP_STATUS_CODE_LEN);
	const char* _status = _status_code_text[status_code];
	memcpy(http_server->status_code, _status, strlen(_status));
}

void http_set_response_body(
		HTTP_Server* http_server,
		const char* body)
{
	// prepend the status code before adding the body
	memset(http_server->response, 0, HTTP_RESPONSE_BODY_LEN);
	memcpy(http_server->response, http_server->status_code, strlen(http_server->status_code));

	// add response body (while accounting for the space we already took up by the response code
	// NOTE: the -5 is for the four extra \r\n\r\n characters and a null terminator
	size_t max_len = (size_t)HTTP_RESPONSE_BODY_LEN - strlen(http_server->status_code) - 5;
	strncat(http_server->response, body, max_len);
	strncat(http_server->response, "\r\n\r\n", 5);
}
