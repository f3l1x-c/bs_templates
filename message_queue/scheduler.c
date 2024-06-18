//
// Created by Marcel on 18.06.24.
//

#include <errno.h>
#include <fcntl.h>
#include <mqueue.h>
#include <stdboolh>
#include <poll.h>
#include <stdio.h>
#include <stdlib.h>

/*
*     Create Message Queue with specific permission, attributes and open the queue
*     mq_open:  returns -1 and errno on error => EACCES, EEXIST 
* */
bool create_message_queue(const char* name) {
    char buffer[256];
	const int oflag = O_CREAT | O_EXCL | O_RDWR;
	const mode_t permissions = S_IRUSR | S_IWUSR; // 600
	const struct mq_attr attr = { .mq_maxmsg = 10, .mq_msgsize = sizeof(buffer) };
	const mqd_t mq = mq_open(name, oflag, permissions, &attr);
	if (mq == -1){
        return false;
    }
	mq_close(mq);
	return true;
}

/*
*     Get messages from the queue
*     poll => wait on file descriptor for some event
*     mq_getattr:  returns -1 and errno on error => EBADF 
*     poll: returns -1 on error, else non negative value
*     mq_receive: returns -1 and errno on error => EAGAIN
* */
void get_messages(){
    char buffer[256];
    struct mq_attr attr;
	if(mq_getattr(mq, &attr) == -1){
        fprintf(stderr, "Error getting message queue attributes\n");
    }
	size_t msg_size = attr.mq_msgsize;
	unsigned int priority;

	struct pollfd poll_fds[1];
	poll_fds->fd = mq;
	poll_fds->events = POLLIN;

	while (1) {
		//check if new messages arrive
		if (poll(poll_fds, 1, -1) > 0) {

			//get message with highest priority
			if (mq_receive(mq, buffer, msg_size, &priority) == -1) {
				fprintf(stderr, "Error receiving message\n");
                return EXIT_FAILURE;
			}
		}
	}
}

/*
*     Example Main
* */
int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: ./%s \"/csXXXX\"\n", argv[0]);
		return EXIT_FAILURE;
	}

	// Get name of message queue
	char* queue_name = argv[1];
	char buffer[5];

	// Open the message queue
	if (!create_message_queue(queue_name)) {
        fprintf(stderr, "Error creating message queue!\n");
		return EXIT_FAILURE;
	}

    get_messages();

	//close and unlink mq
	printf("\nShutting down...\n");
	mq_unlink(argv[1]);
	mq_close(mq);

	return EXIT_SUCCESS;
}