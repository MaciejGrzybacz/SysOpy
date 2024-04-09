//
// Created by maciejgrzybacz on 08.04.24.
//

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
int global=0;
void printpid() {
    printf("Parent PID: %d, PID: %d \n", (int) getppid(), (int) getpid());
}

int main(int argc, char** argv) {
    int local=0;
    if (argc<2) {
        printf("To few arguments!");
        return 1;
    }
    int n= atol(argv[1]);
    for(int i=0; i<n; i++) {

        pid_t child_pid=fork();
        if(child_pid==-1) {
            printf("Fork error!");
            return 1;
        }
        if(child_pid==0) {
            local++;
            global++;
            printf("%d %d\n",local,global);
            printpid();
            exit(0);
        }
    }

    for(int i=0; i<n; i++) {
        int status;
        wait(&status);
    }
    printf("Created %d processes succesfully\n", n);
    printf("%d %d\n",local,global);

}