//
// Created by maciejgrzybacz on 08.04.24.
//

#include <signal.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char** argv) {
    if (argc <3) {
        printf("Usage: %s <pid> <signal>\n", argv[0]);
        return 1;
    }
    int pid = atoi(argv[1]);
    int n = atoi(argv[2]);

    for(int i=0; i<n; i++) {
        kill(pid, SIGUSR1);
        sleep(1);
    }

    kill(pid, SIGKILL);
    return 0;

}