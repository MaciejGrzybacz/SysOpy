#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int calc_sum(int x) {
    return x + x;
}

int main() {
    int pfd[2], cfd[2];
    if (pipe(pfd) == -1 || pipe(cfd) == -1) {
        perror("pipe failed");
        return 1;
    }

    for (int i = 0; i < 5; i++) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            return 1;
        } else if (pid == 0) {
            // Child process
            close(pfd[1]);
            close(cfd[0]);

            int x;
            if (read(pfd[0], &x, sizeof(int)) != sizeof(int)) {
                perror("read failed");
                exit(1);
            }
            printf("Received %d\n", x);

            x = calc_sum(x);

            if (write(cfd[1], &x, sizeof(int)) != sizeof(int)) {
                perror("write failed");
                exit(1);
            }

            close(pfd[0]);
            close(cfd[1]);
            exit(0);
        }
    }

    close(pfd[0]);
    close(cfd[1]);

    for (int i = 0; i < 5; i++) {
        if (write(pfd[1], &i, sizeof(int)) != sizeof(int)) {
            perror("write failed");
        }

        int x;
        if (read(cfd[0], &x, sizeof(int)) != sizeof(int)) {
            perror("read failed");
        } else {
            printf("x+x = %d\n", x);
        }
    }

    close(pfd[1]);
    close(cfd[0]);

    for (int i = 0; i < 5; i++) {
        int status;
        wait(&status);
    }

    return 0;
}
