//
// Created by felix on 19.06.24.
//

#ifndef FIFO_FUNCTIONS_H
#define FIFO_FUNCTIONS_H

#include <fcntl.h>
#include <poll.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

void mkfifo_safe(const char* path, mode_t mode) {
    if(mkfifo(path, mode) == -1) {
        perror("mkfifo");
        exit(EXIT_FAILURE);
    }
}
int open_safe(const char* path, int o_flag) {
    int fd = open(path, o_flag);
    if(fd == -1) {
        perror("open");
        exit(EXIT_FAILURE);
    }
    return fd;
}

void poll_safe(struct pollfd fds[], int size, int timeout) {
    int ret = poll(fds, size, timeout);
    if(ret == -1) {
        perror("poll");
        exit(EXIT_FAILURE);
    }
}

void close_all(int fifo_fds[], size_t size) {
    for(int i = 0; i < size; i++) {
        close(fifo_fds[i]);
    }
}

#endif //FIFO_FUNCTIONS_H
