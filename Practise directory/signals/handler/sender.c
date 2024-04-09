//
// Created by maciejgrzybacz on 09.04.24.
//

#include <stdio.h>
#include <signal.h>
#include <unistd.h>
void handler(int signo) {
    printf("Received signal with number: %d", signo);
}
int main(int argc, char** argv) {
    signal(SIGUSR1,handler);
    raise(SIGUSR1);
    return 0;
}