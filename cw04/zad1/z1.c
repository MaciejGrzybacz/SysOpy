//
// Created by maciejgrzybacz on 20.03.24.
//
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char* argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <num_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]);
    if (num_processes <= 0) {
        fprintf(stderr, "Invalid number of processes\n");
        return 1;
    }

    for (int i = 0; i < num_processes; i++) {
        pid_t child_pid = fork();
        if (child_pid == -1) {
            perror("fork");
            return 1;
        } else if (child_pid == 0) {
            printf("Child process: %d, Parent process: %d\n", getpid(), getppid());
            exit(0);
        }
//        else {
//            printf("Parent process: %d\n", getpid());
//            int status;
//            wait(&status);
//        }
    }

    for (int i = 0; i < num_processes; i++) {
        int status;
        wait(&status);
    }
    printf("argv[1]: %s\n", argv[1]);
    return 0;
}
