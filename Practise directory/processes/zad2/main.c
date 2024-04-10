#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int fd[2];
int d = 0;
int active=1;

void sigusr1_handler(int signum) {
    write(fd[1], &d, sizeof(int));
    d++;
}

void sigint_handler(int signum) {
    active= active==1 ?0 : 1;
}

int main(int argc, char** argv) {
    pid_t pid1, pid2;
    signal(SIGINT, sigint_handler);

    if (pipe(fd) == -1) {
        perror("pipe error");
        exit(EXIT_FAILURE);
    }

    pid1 = fork();
    if (pid1 == 0) {
        close(fd[0]);
        signal(SIGUSR1, sigusr1_handler);
        while (1) {
            pause();
        }
    }

    pid2 = fork();
    if (pid2 == 0) {
        int num;
        close(fd[1]);
        while (1) {
            if (read(fd[0], &num, sizeof(int)) > 0) {
                printf("Value of d: %d\n", num);
            }
        }
    }
    while (1) {
        if (active) {
            kill(pid1, SIGUSR1);
            printf("Sent SIGUSR1\n");
        }
        sleep(1);
    }
}