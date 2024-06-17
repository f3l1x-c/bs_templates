//
// Created by Marcel on 17.06.24.
//

#include <stdio.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MSG_SIZE 1024

/*
*     Establish connection to server to communicate over socket.
*     - socket: returns value under 0 if conversion failed.
*     - connect: returns -1 and errno on error => EADDRINUSE
* */
int establish_connection(long port){
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        fprintf(stderr, "socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_in server_addr;
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(port);

    // initiate a connection on a socket
    if (connect(server_socket, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        fprintf(stderr, "connect");
        close(server_socket);
        exit(EXIT_FAILURE);
    }
    return server_socket;
}


/*
 *     Example Main:
 * */
int main(int argc, char *argv[]) {
    if (argc < 3) {
        fprintf(stderr, "Usage: %s <port> <username>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // convert port from given argument
	long port = validate_l_argument(argv[1]);

    // get username
    char *username = argv[2];

    int server_socket = establish_connection(port);

    //send username to server
    send(server_socket, username, MSG_SIZE, 0);

    /* if we want to print each message on each client
    pthread_t receiveThread;
    if (pthread_create(&receiveThread, NULL, receiveMessages, &server_socket)!=0){
        fprintf(stderr, "error: creating receive thread\n");
        exit(EXIT_FAILURE);
    }
    */

    // send messages to server
    while(1){
        char message[MSG_SIZE];
        fgets(message, MSG_SIZE, stdin);
        send(server_socket, message, strlen(message),0);

        if(strcmp(message, "/shutdown\n")==0){
            break;
        }
    }

    //pthread_cancel(receiveThread);
    //pthread_join(receiveThread, NULL);
    close(server_socket);
}

/*
*     If each client should print sended message.
* */
void* receiveMessages(void* socket){
    int clientSocket = *(int*)socket;
    char buffer[MSG_SIZE];

    while(1){
        ssize_t bytesReceived = recv(clientSocket, buffer, MSG_SIZE,0);
        if(bytesReceived <=0){
            break;
        }
        buffer[bytesReceived]='\0';
        printf("%s", buffer);
    }
    return NULL;
}