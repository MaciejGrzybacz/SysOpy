//
// Created by maciejgrzybacz on 10.04.24.
//
#include <stdio.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

struct pak{
    int i;
    char lit;
};

int main (int argc, char** argv){
    int w1;
    struct pak ob1;
    mkfifo("potok1", S_IRWXU);

    while (1){
        w1 = open("potok1", O_RDONLY);
        read(w1, &ob1, sizeof(ob1));
        printf("Received: %d %c\n",ob1.i,ob1.lit);
        fflush(stdout);
    }
    //unused because of while(1) but in other cases necessery
    close(w1);
    return 0;
}
