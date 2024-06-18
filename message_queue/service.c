//
// Created by Marcel on 18.06.24.
//

#include <mqueue.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>

/*
*     Send messages to message queue
*     mq_open:  returns -1 and errno on error => EACCES, EEXIST
*     mq_send:  returns -1 and errno on error => EAGAIN
* */
void send_message(char *buffer, int priority, char *path){
    // Open the message queue - O_WRONLY, O_RDONLY, O_RDWR
	mqd_t mq = mq_open(path, O_WRONLY);
	if (mq == -1) {
		fprintf(stderr, "Error opening message queue!\n");
        exit(EXIT_FAILURE);
	}

	// Send the message
	if (mq_send(mq, buffer, strlen(buffer), priority) == -1) {
		mq_close(mq);
		fprintf(stderr, "Error sending message to queue!\n");
		exit(EXIT_FAILURE);
	}
}

/*
*     Example Main
* */
int main(int argc, char* argv[]) {
	if (argc != 3) {
		printf("Usage: <message> <priority>\n");
		return EXIT_FAILURE;
	}

    int priority = atoi(argv[2]);

	char *path = "/csxxxxxx";

    // call function
	send_message(argv[1], priority, path);

	// Close the message queue
	mq_close(mq);

	return EXIT_SUCCESS;
}