#ifndef HTTP_SERVER_H
#define HTTP_SERVER_H

#define HTTP_HEADER_VERSION "HTTP/1.1 "
#define HTTP_OK_RESPONSE    "200 Ok\r\n\r\n"  
 
#define  DFAULT_PORT 6969 
#define  LISTEN_BACKLOG 10   

#define  HTTP_DIE  exit(EXIT_FAILURE) 
typedef struct  sockaddr sockaddr  ;  
#define SA  sockaddr 

#include <stdbool.h> 

typedef struct HTTP_Server {
	int socket;
	int port;	
} HTTP_Server;


typedef struct  {  
     int port ;  
     int listen_blog   ;  
     _Bool  show_available_routes ; 
} HTTP_Argparser ; 


/* Handle  argument options **/
void argument_parser ( int __argc , char *const *__argv  , HTTP_Argparser *  ha); 

void init_server(HTTP_Server* http_server, int *port); 

void http_error (const char * __fmt  , ...) ; 

#endif
