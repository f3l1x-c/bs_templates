//
// Created by john on 17.06.24.
//


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

#define BUFFER_SIZE 1024

int main() {
    int pipefd[2];
    pid_t pid;
    char buffer[BUFFER_SIZE];
    const char* message = "Hello from parent to child via pipe!";

    // Create the pipe
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close the write end of the pipe

        // Read from the pipe
        ssize_t bytesRead = read(pipefd[0], buffer, BUFFER_SIZE - 1);
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }
        
        buffer[bytesRead] = '\0';
        printf("Child received: %s\n", buffer);

        close(pipefd[0]);
    } else { // Parent process
        close(pipefd[0]); // Close the read end of the pipe

        // Write to the pipe
        ssize_t bytesWritten = write(pipefd[1], message, strlen(message));
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]); // Close the write end of the pipe

        // Wait for child process to finish
        wait(NULL);
    }

    return 0;
}
