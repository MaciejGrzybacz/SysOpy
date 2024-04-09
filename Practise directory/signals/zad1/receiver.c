//
// Created by maciejgrzybacz on 08.04.24.
//
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

void signal_processor(int signum) {
    printf("Received signal %d\n", signum);
}
int main(int argc, char** argv) {
    printf("Receiver PID: %d\n", getpid());

    while(1) {
        printf("Waiting for signal...\n");
        signal(SIGUSR1, signal_processor);
        pause();
    }
    return 0;
}