//
// Created by john on 17.06.24.
//

#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <time.h>

volatile sig_atomic_t sig = 0;  // threadsafe flag

void signal_handler(int signum) {
	sig = signum;
}

int main(void) {
	// use sigaction to register signal handler
	struct sigaction sa;
	sa.sa_handler = signal_handler;
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = 0;

	// print pid for sending signals instead of running:
	// ps -C signal_handler
	printf("My pid is: %d\n", getpid());

	// allow required signals
	if(sigaction(SIGINT, &sa, NULL) == -1 || sigaction(SIGCONT, &sa, NULL) == -1) {
		// SIGSTOP and SIGKILL are not catchable signals
		perror("sigaction");
		return EXIT_FAILURE;
	}

	while(true) {
		nanosleep((const struct timespec[]){ { 0, 100000000L } }, NULL);
		switch(sig) {
			case 0: break;
			case 2:
				printf("Received SIGINT\n");
				sig=0;
				break;
			case 18:
				printf("Received SIGCONT\n");
				sig=0;
				break;
			default:
				printf("Received unknown SIG\n");
				sig = 0;
				break;
		}
	}

	return EXIT_SUCCESS;
}