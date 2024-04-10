//
// Created by maciejgrzybacz on 10.04.24.
//
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>

char* msg_handler(char* message) {
    printf("message %s\n",message);
    return "abcdefghij";
}

int main() {
    char np_path[]="./fifo";
    int fd=open(np_path,O_RDONLY);
    if (fd==-1) {
        printf("Error while opening a pipe");
        return 1;
    }
    char sender_message[7];
    read(fd,sender_message,7*sizeof(char));
    close(fd);
    char* message=msg_handler(sender_message);

    fd=open(np_path,O_WRONLY);
    if (fd==-1) {
        printf("Error while opening a pipe");
        return 1;
    }
    write(fd,message,10*sizeof (char));
    close(fd);
    printf("Message sent\n");
    return 0;
}