//
// Created by Marcel on 18.06.24.
//

#include <errno.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <ctype.h>
#include <fcntl.h>

struct Buffer
{
    size_t cnt;
    char buffer[];
};

/*
*     _Example Main_
*     shm_open:   returns -1 on error
*     ftruncate:  returns -1 and errno on error => EACCES, EFAULT
*     mmap:       returns MAP_FAILED ((void *)-1) and errno on error => EACCESm EAGAIN
*     munmap:     returns -1 and errno on failure=> EINVAL
*     shm_unlink: returns -1 on error
* */
int main(int argc, char *argv[])
{
    if (argc != 4)
    {
        fprintf(stderr, "Usage: <path>\n");
        return EXIT_FAILURE;
    }

    char *shm_path = argv[1];

    // create or open shared memory object
    int fd = shm_open(shm_path, O_CREAT | O_EXCL | O_RDWR, S_IRUSR | S_IWUSR);
    if (fd == -1){
        fprintf(stderr, "Error opening shared memory!\n");
        return EXIT_FAILURE;
    }

    // set size to size of structure
    if (ftruncate(fd, sizeof(struct Buffer)) == -1){
        fprintf(stderr, "Error trucate size!\n");
        return EXIT_FAILURE;
    }

    // pointer to mapped area - map files into memory
    struct Buffer *shared_buffer = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    if (shared_buffer == MAP_FAILED)
    {
        fprintf(stderr, "Error mapping object into callers space address!\n");
        return EXIT_FAILURE;
    }

    // Clean up shared memory - unmap files or devices into memory
    if (munmap(shared_buffer, size) == -1)
    {
        perror("munmap");
        return EXIT_FAILURE;
    }

    shm_unlink(shmpath);

    return EXIT_SUCCESS;
}