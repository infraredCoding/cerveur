#include <stdlib.h> 
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdarg.h>
#include <stdbool.h> 
#include <errno.h>
#include <string.h> 
#include <unistd.h> 
#include <getopt.h>  



#include "HTTP_Server.h" 

void argument_parser ( int argc  , char * const  *argv  , HTTP_Argparser  *http_argparse )  {
    
    if  (argc  == 1 ) 
        return  ; 
   
    int  option_parser  =0  ;  
    struct  option  http_arguments_options [] = { 
        {"version"        , no_argument        ,  0 , 'v'} ,   
        {"port"           , required_argument  ,  0 , 'p'} , 
        {"listen-backlog" , required_argument  ,  0 , 'l'} , 
        {"show-routes"    , no_argument        ,  0 , 'r'}   
    }; 
    
    while ( (option_parser  = getopt_long(argc, argv , "vp:l:r" , http_arguments_options , 0  ) ) != -1 ) 
    {
        switch (option_parser) 
        {
            case 'v' : 
                fprintf(stdout  , "Cerver  version 1.0 \n" ) ; 
                break ; 
            case 'p' : 
                 http_argparse->port = strtol (optarg , NULL , 10 ) ; 
                break ; 
            case 'l':  
                 http_argparse->listen_blog = strtol (optarg , NULL , 10 ) ; 
                break; 
            case 'r': 
               http_argparse->show_available_routes = true  ; 
               break ; 
            default :
               fprintf(stderr, "unrecongnize flags argument %c \n" ,argv[optind])  ; 
               exit(EXIT_FAILURE) ; 
        }
    } 

}



void  http_error ( const char *  fmt , ... )  { 
    
    int catch_error  = errno  ;  
     
    va_list  ap  ; 
    va_start (ap , fmt) ;  
    vfprintf(stderr , fmt , ap ) ;  
    fprintf(stderr , "\n") ;  
    fflush(stderr) ; 

    if (catch_error  != 0 )  
    { 
        fprintf (stderr ,"HTTP_ERR: code [ %i ] : < %s > \n", catch_error, strerror(catch_error)) ; 
        fflush(stderr) ;
        HTTP_DIE ;  
    }

    va_end(ap) ; 
    
}
void init_server(HTTP_Server * http_server, int *port) {

    if  ( port ==  NULL  || *port == 0 ) 
    {
        http_server->port = DFAULT_PORT ; 
    } else {  
        http_server->port = *port ;  
    } 

	http_server-> socket  =  socket(AF_INET, SOCK_STREAM, 0);
    
    if  ( http_server->socket  ==  -1)  
    { 
        http_error("HPPT_SCKT_CREATION_FAIL %i" , http_server->socket) ;
        HTTP_DIE ;  
    } 

	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(*port);
	server_address.sin_addr.s_addr = INADDR_ANY;

	bind(http_server->socket , (SA*) &server_address, sizeof(server_address));

	listen(http_server->socket, LISTEN_BACKLOG);

	printf("HTTP Server Initialized\nPort: %d\n", http_server->port);
}
