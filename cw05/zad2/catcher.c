//
// Created by maciejgrzybacz on 31.03.24.
//
#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>

int status = -1;
int status_changes = 0;

void handler(int argument){
    status_changes++;
    status = argument;
}

void SIGUSR1_action(int signo, siginfo_t *info, void *extra){
    int int_val = info->si_int;
    printf("Received status: %d from pid: %d\n", int_val, info->si_pid);
    handler(int_val);
    kill(info->si_pid, SIGUSR1);
}

int main() {
    printf("Catcher PID: %d\n", getpid());

    struct sigaction action;
    action.sa_sigaction = SIGUSR1_action;
    action.sa_flags = SA_SIGINFO;
    sigemptyset(&action.sa_mask);
    sigaction(SIGUSR1, &action, NULL);

    while(1) {
        switch(status){
            case 1:
                printf("Received signal %d\n",1);
                for(int i = 1; i <= 100; i++){
                    printf("%i, ", i);
                }
                printf("\n");
                status = -1;
                break;
            case 2:
                printf("So far status has changed %d times\n", status_changes);
                status = -1;
                break;
            case 3:
                printf("Received signal %d\n Exiting...",3);
                exit(0);
        }
    }
}
