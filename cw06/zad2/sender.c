#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/wait.h>

int main() {
    char receiver_path[] = "./receiver";
    char input_fifo_path[] = "./input_fifo";
    char output_fifo_path[] = "./output_fifo";
    double a, b;

    printf("Enter a: ");
    scanf("%lf", &a);
    printf("Enter b: ");
    scanf("%lf", &b);

    mkfifo(input_fifo_path, 0666);
    mkfifo(output_fifo_path, 0666);

    pid_t pid = fork();
    if (pid == 0) {
        execl(receiver_path, receiver_path, NULL);
        perror("Failed to execute receiver");
        exit(1);
    } else if (pid < 0) {
        perror("Failed to fork");
        exit(1);
    }

    int ifd = open(input_fifo_path, O_WRONLY);
    int ofd = open(output_fifo_path, O_RDONLY);

    write(ifd, &a, sizeof(double));
    write(ifd, &b, sizeof(double));
    close(ifd);

    double result;
    read(ofd, &result, sizeof(double));
    close(ofd);

    printf("Result: %f\n", result);

    wait(NULL);
    return 0;
}
