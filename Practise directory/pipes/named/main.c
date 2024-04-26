#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int calc_sum(int x) {
    return x + x;
}

int main() {
    char fifo_path[]="./fifo";
    mkfifo(fifo_path,0666);
    pid_t pid=fork();
    if(pid==-1) {
        printf("error");
        return 1;
    } else if (pid==0) {
        int fd=open(fifo_path,O_RDONLY);
        int x;
        read(fd,&x,sizeof(int));
        close(fd);
        x*=x;
        fd=open(fifo_path,O_WRONLY);
        write(fd,&x, sizeof(int));
        exit(0);
    }

    int fd=open(fifo_path,O_WRONLY);
    int x=4;
    if (write(fd, &x, sizeof(int)) == sizeof(int)){
        printf("succesfully writed %d\n",x);
    }
    close(fd);
    fd=open(fifo_path,O_RDONLY);
    read(fd,&x, sizeof(int));
    int status;
    wait(&status);
    printf("Received %d",x);
    return 0;
}
