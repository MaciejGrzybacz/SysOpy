#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main(int argc, char** argv) {
    if (argc < 4) {
        printf("Usage: %s <a> <b> <n>\n", argv[0]);
        return 1;
    }

    double a = atof(argv[1]);
    double b = atof(argv[2]);
    int n = atoi(argv[3]);

    char * path = "./fifo";
    int status = mkfifo(path, 0606);
    if (status == -1) {
        printf("Error while creating fifo");
        return 1;
    }

    int fd = open(path, O_WRONLY);
    if (fd == -1) {
        printf("Error while opening fifo in %s", argv[0]);
        return 2;
    }

    write(fd, &a, sizeof(double));
    write(fd, &b, sizeof(double));
    write(fd, &n, sizeof(int));
    close(fd);

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        return 3;
    } else if (pid == 0) {
        execl("./receiver", "receiver", NULL);
        perror("exec");
        exit(EXIT_FAILURE);
    } else {
        int status;
        wait(&status);
    }

    fd = open(path, O_RDONLY);
    if (fd == -1) {
        printf("Error while opening fifo in %s", argv[0]);
        return 2;
    }

    double result;
    read(fd, &result, sizeof(double));
    printf("Result: %f\n", result);

    close(fd);

    return 0;
}
