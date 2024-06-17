//
// Created by Marcel on 17.06.24.
//

#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/socket.h>

/*
*     Initialization of the socket -> create, bind and then listen on socket.
*     - socket: returns value under 0 if conversion failed.
*     - bind:   returns -1 and errno on error => EADDRINUSE
*     - listen: returns -1 and errno on error => EADDRINUSE
*     - setsockopt: returns -1 on error
* */
int initialize_socket(long port) {
	// create new Socket - create an endpoint for communication
	int sock_fd = socket(PF_INET, SOCK_STREAM, 0);
	if (sock_fd < 0) {
		printf(stderr, "Cannot create socket!");
		return EXIT_FAILURE;
	}

    // Allow socket reuse
    int opt = 1;
    if (setsockopt(sock_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt))) {
        fprintf(stderr, "Setsockopt failed");
        close(sock_fd);
        exit(EXIT_FAILURE);
    }

	// bind server to given attr
	struct sockaddr_in serv_addr;
	// optional: fill memory of serv_addr with 0
	// memset(&serv_addr, 0, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(port);

	// bind a name to a socket - Sockets created with the socket() function are initially unnamed;
	// they are identified only by their address family.
	if (bind(sock_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1 || errno == EADDRINUSE) {
		fprintf(stderr, "Cannot bind socket!");
		close(sock_fd);
		return EXIT_FAILURE;
	}

	// listen for connections on a socket
	if (listen(sock_fd, 1) == -1) {
		fprintf(stderr, "Cannot listen for connections on a socket!");
		close(sock_fd);
		return EXIT_FAILURE;
	}

	printf("Listening on port %ld.\n", port);
	return sock_fd;
}

/*
*     Read from socket, after accepting a connection on it.
*     - read: returns -1 and errno on error => EAGAIN (file neither pipe/fifo/socket).
*     - strtod: returns 0 if no conversion
* */
double read_from_socket(int sock_fd, char *buffer) {
	ssize_t bytes_received = read(sock_fd, buffer, sizeof(buffer));
	if (bytes_received < 0) {
		fprintf(stderr, "Cannot read from socket!");
		close(sock_fd);
	}

	// convert received data to double
	char* end;
	double received = strtod(buffer, &end);
	if (*end != '\n' && *end != '\0') {
		size_t len = strlen(buffer);
		// convert new line character with null termination to print buffer with text in one line
		if (len > 0 && buffer[len - 1] == '\n') {
			buffer[len - 1] = '\0';
		}
	}
    return received;
}

/*
*     Accept connection on socket and communicate over it.
*     - accept: returns -1 and errno on error => EAGAIN or EWOULDBLOCK
* */
void communication(int sock_fd){
    struct sockaddr_in client_addr;
	socklen_t client_addr_len = sizeof(client_addr);
	char* buffer = malloc(sizeof(char*));

	while (1) {
		// accept a connection on a socket - creates a new connected socket and returns a new file
		// descriptor referring to that socket.
		int new_sock_fd = accept(sock_fd, (struct sockaddr*)&client_addr, &client_addr_len);
		printf("Established connection!\n");
		if (new_sock_fd < 0) {
			fprintf(stderr, "Cannot Accept Connection!");
			continue;
		}

        /* Example - read from socket
		while (1) {
			// optional: fill memory of buffer with 0 before each read
			// memset(buffer, 0, sizeof(buffer));
			
            
            read message from the client - example use
			if(read_from_socket(new_sock_fd, &buffer) < 1.0){
                break;
            }
            
		}
        */
	}
    free(buffer);
}


/*
 *     Example Main:
 * */
int main(int argc, char** argv) {
	if (argc != 2) {
		printf("usage: ./server <port number>\n");
		return EXIT_FAILURE;
	}

	// convert port from given argument
	long port = validate_l_argument(argv[1]);

	// initialize socket with arguments
	int sock_fd = initialize_socket(port);

    // read from socket
	communication(sock_fd);

	// close socket and connection
	close(sock_fd);
	printf("Connection closed.\n");
	return EXIT_SUCCESS;
}