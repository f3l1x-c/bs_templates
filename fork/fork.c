//
// Created by Marcel on 18.06.24.
//

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

/*
*     Forks a child process and implements logic for both child and parent
*     - fork: returns -1 and errno on failure => EAGAIN - returns process ID of child in parent process and 0 in child if successful
*     - waitpid: returns -1 on error - returns process ID of child if successful
* */
void parent_child_logic(){
    // Fork
	pid_t child_a;
	if ((child_a = fork()) == 0) {
		// Child A
		// do smth
		exit(EXIT_SUCCESS);
	} 
    else if (child_a < 0) {
		fprintf(stderr, "Error forking!\n");
		exit(EXIT_FAILURE);
	}
    else{
        pid_t child_b;
	    if ((child_b = fork()) == 0) {
		    // Child B
		    // do smth
		    exit(EXIT_SUCCESS);
	    } 
        else if (child_b < 0) {
		    fprintf(stderr, "Error forking!\n");
            exit(EXIT_FAILURE);	    
        }
        else{
            // Wait for both child processes to finish
	        if (waitpid(child_a, NULL, 0) == -1 || waitpid(child_b, NULL, 0) == -1) {
		        fprintf(stderr, "Error waitpid\n");
		        exit(EXIT_FAILURE);
	        }
		}
    }
}


/*
*     _Example Main_
*     parent_child_logic: creates a child process and implements logic for child and parent
* */
int main(int argc, char* argv[]) {
	if (argc != 4) {
		fprintf(stderr, "Usage: %s \n", argv[0]);
		return EXIT_FAILURE;
	}

    // call function
    parent_child_logic();

	return EXIT_SUCCESS;
}