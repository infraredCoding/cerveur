#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <fcntl.h>
#include <unistd.h>
#include <errno.h>

#include "HTTP_Server.h"
#include "Routes.h"
#include "Response.h"

int main() {
	// initiate HTTP_Server
	HTTP_Server http_server;
	init_server(&http_server, 6969);

	int client_socket;
	
	// registering Routes
	struct Route * route = initRoute("/", "index.html"); 
	addRoute(route, "/about", "about.html");


	printf("\n====================================\n");
	printf("=========ALL AVAILABLE ROUTES========\n");
	// display all available routes
	inorder(route);

	while (1) {
		char client_msg[4096] = "";

		client_socket = accept(http_server.socket, NULL, NULL);

		read(client_socket, client_msg, 4095);
		printf("%s\n", client_msg);

		// parsing client socket header to get HTTP method, route
		char *method = "";
		char *urlRoute = "";

		char *client_http_header = strtok(client_msg, "\n");
			
		printf("\n\n%s\n\n", client_http_header);

		char *header_token = strtok(client_http_header, " ");
		
		int header_parse_counter = 0;

		while (header_token != NULL) {

			switch (header_parse_counter) {
				case 0:
					method = header_token;
				case 1:
					urlRoute = header_token;
			}
			header_token = strtok(NULL, " ");
			header_parse_counter++;
		}

		printf("The method is %s\n", method);
		printf("The route is %s\n", urlRoute);


		char template[100] = "";
		
		if (strstr(urlRoute, "/static/") != NULL) {
			//strcat(template, urlRoute+1);
			strcat(template, "static/index.css");
			http_set_status_code(&http_server, OK);
		}else {
			struct Route * destination = search(route, urlRoute);
			strcat(template, "templates/");

			if (destination == NULL) {
				strcat(template, "404.html");
				http_set_status_code(&http_server, NOT_FOUND);
			}else {
				strcat(template, destination->value);
				http_set_status_code(&http_server, OK);
			}
		}

		char * response_data = render_static_file(template);
		http_set_response_body(&http_server, response_data);

		send(client_socket, http_server.response, sizeof(http_server.response), 0);
		close(client_socket);
		free(response_data);
	}
	return 0;
}
