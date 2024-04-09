//
// Created by maciejgrzybacz on 31.03.24.
//

#include <stdio.h>
#include <signal.h>
#include <stdlib.h>
#include <string.h>

void handler(int sig) {
    printf("Received SIGUSR1 signal: %d\n", sig);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <option>\n", argv[0]);
        exit(1);
    }

    if (strcmp(argv[1], "none") == 0) {
        signal(SIGUSR1, SIG_DFL);
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "ignore") == 0) {
        signal(SIGUSR1, SIG_IGN);
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "handler") == 0) {
        signal(SIGUSR1, handler);
        raise(SIGUSR1);
    } else if (strcmp(argv[1], "mask") == 0) {
        sigset_t block_mask;
        sigemptyset(&block_mask);
        sigaddset(&block_mask, SIGUSR1);
        sigprocmask(SIG_BLOCK, &block_mask, NULL);
        raise(SIGUSR1);
        sigset_t pending;
        sigpending(&pending);
        if (sigismember(&pending, SIGUSR1)) {
            printf("SIGUSR1 is pending.\n");
        } else {
            printf("SIGUSR1 is not pending.\n");
        }
    } else {
        fprintf(stderr,"Invalid option: %s\n",argv[1]);
    }
    return 0;
}
