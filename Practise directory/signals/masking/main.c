//
// Created by maciejgrzybacz on 09.04.24.
//
#include <signal.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    struct sigaction action;
    action.sa_flags=0;
    sigemptyset(&action.sa_mask);
    sigaddset(&action.sa_mask,SIGUSR1);
    sigset_t pending;
    sigpending(&pending);
    if(sigismember(&pending,SIGUSR1)){
        printf("SIGUSR1 is pending...\n");
    } else {
        printf("SIGUSR1 is not pending\n");
    }
    return 0;
}
