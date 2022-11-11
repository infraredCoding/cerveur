#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

typedef struct HTTP_Server {
	int socket;
	int port;	
} HTTP_Server;

void init_server(HTTP_Server* http_server, int port);

#endif
