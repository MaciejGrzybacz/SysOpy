//
// Created by maciejgrzybacz on 02.03.24.
//

#include <fcntl.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

char* read_from_file(const char* filename, size_t lenght) {
    char* buffer;
    int fd;
    ssize_t bytes_read;

    // Allocate the buffer
    buffer = (char*) malloc(lenght);
    if (buffer==NULL) {
        return NULL;
    }

    // Open the file
    fd=open(filename,O_RDONLY);
    if (fd==-1){
        // Open failed. Deallocate buffer before returning.
        free(buffer);
        return NULL;
    }

    // Read the data
    bytes_read= read(fd, buffer, lenght);
    if (bytes_read != lenght) {
        // Read failed. Deallocate buffer and close fd before returning.
        free(buffer);
        close(fd);
        return NULL;
    }

    // Everything is fine. CLose the file and return the buffer.
    close(fd);
    return buffer;
}