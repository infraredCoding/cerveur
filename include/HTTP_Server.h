#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#include <string.h>
#include <stddef.h>

#define HTTP_STATUS_CODE_LEN 50
#define HTTP_RESPONSE_BODY_LEN 4096

// note: please keep these in the same order as _status_code_text
// as that's the corresponding lookup table
enum http_status_code_e
{
	OK = 0,
	CREATED,
	BAD_REQUEST,
	UNAUTHORIZED,
	FORBIDDEN,
	NOT_FOUND,
	INTERNAL_ERROR,
	NOT_IMPLEMENTED
};

// defined in HTTP_Server.c
extern const char _status_code_text[8][50];

typedef struct HTTP_Server {
	int socket;
	int port;	
	char status_code[HTTP_STATUS_CODE_LEN];
	char response[HTTP_RESPONSE_BODY_LEN];
} HTTP_Server;

void init_server(HTTP_Server* http_server, int port);

void http_set_status_code(
		HTTP_Server* http_server, 
		const enum http_status_code_e);

void http_set_response_body(
		HTTP_Server* http_server,
		const char* body);

#endif
