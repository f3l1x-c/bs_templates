#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <errno.h>

#define FIFO_PATH_PREFIX "/tmp/my_fifo_path"
#define FIFO_NAME "fifo_name"
#define BUFFER_SIZE 1024

int main() {
    pid_t pid;
    char buffer[BUFFER_SIZE];
    const char* message = "Hello from parent to child via named pipe!";

    if (mkdir(FIFO_PATH_PREFIX , 0777) == -1 && errno != EEXIST) {
        fprintf(stderr,"error:mkdir\n");
        exit(EXIT_FAILURE);
    }
    // Create the named pipe (FIFO)
    char fifo_path[100];
    snprintf(fifo_path, sizeof(fifo_path), "%s/%s", FIFO_PATH_PREFIX, FIFO_NAME);
    if (mkfifo(fifo_path, 0666) == -1) {
        fprintf(stderr, "error:mkfifo\n");
        exit(EXIT_FAILURE);
    }

    // Fork a child process
    pid = fork();
    if (pid == -1) {
        fprintf(stderr,"fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        int fd;

        // Open the FIFO for reading
        char fifo_path[100];
        snprintf(fifo_path, sizeof(fifo_path), "%s/%s", FIFO_PATH_PREFIX, FIFO_NAME);
        fd = open(fifo_path, O_RDONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Read from the FIFO
        ssize_t bytesRead = read(fd, buffer, BUFFER_SIZE - 1);
        if (bytesRead == -1) {
            perror("read");
            exit(EXIT_FAILURE);
        }

        buffer[bytesRead] = '\0'; // Null-terminate the string
        printf("Child received: %s\n", buffer);

        close(fd); // Close the FIFO

    } else { // Parent process
        int fd;

        // Open the FIFO for writing
        char fifo_path[100];
        snprintf(fifo_path, sizeof(fifo_path), "%s/%s", FIFO_PATH_PREFIX, FIFO_NAME);
        fd = open(fifo_path, O_WRONLY);
        if (fd == -1) {
            perror("open");
            exit(EXIT_FAILURE);
        }

        // Write to the FIFO
        ssize_t bytesWritten = write(fd, message, strlen(message));
        if (bytesWritten == -1) {
            perror("write");
            exit(EXIT_FAILURE);
        }

        close(fd); // Close the FIFO

        // Wait for child process to finish
        wait(NULL);

        // Remove the named pipe
        if (unlink(fifo_path) == -1) {
            perror("unlink");
            exit(EXIT_FAILURE);
        }
        rmdir(FIFO_PATH_PREFIX);
    }

    return 0;
}
