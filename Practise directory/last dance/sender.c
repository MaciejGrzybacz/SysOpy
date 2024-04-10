//
// Created by maciejgrzybacz on 10.04.24.
//
#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/wait.h>


int main(int argc, char** argv) {
    if (argc<2) {
        printf("Invalid number of arguments\n");
        return 1;
    }
    int pid=atol(argv[1]);
    char np_path[]="./fifo";
    char receiver_path[]="./receiver";
    if(mkfifo(np_path,0666)==-1) {
        printf("Error while creating named pipe\n");
        return 2;
    }

    pid_t child_pid=fork();
    if(child_pid==-1) {
        printf("Error while forking");
        return 3;
    } else if (child_pid==0) {

        execl(receiver_path,receiver_path);
        exit(0);
    } else {
        int fd=open(np_path,O_WRONLY);
        if (fd==-1) {
            printf("Error while opening named pipe\n");
            return 3;
        }
        char* sender_message="Message";
        write(fd,sender_message,7*sizeof(char));
        close(fd);
        fd=open(np_path,O_RDONLY);
        if (fd==-1) {
            printf("Error while opening named pipe\n");
            return 3;
        }

        char message[10];
        read(fd,message,10*sizeof(char));
        close(fd);
        printf("%s\n", message);
    }

    int status;
    wait(&status);

    return 0;
}