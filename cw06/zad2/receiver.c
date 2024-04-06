#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

double f(double x) {
    return 4 / (x * x + 1);
}

double calc(double a, double b, int n) {
    double step = (b - a) / n;
    double result = 0.0;
    for (int i = 0; i < n; i++) {
        result += f(a + i * step);
    }
    return result * step;
}

int main(int argc, char** argv) {
    int fd = open("./fifo", O_RDONLY);
    if (fd == -1) {
        printf("Error while opening fifo in %s", argv[0]);
        return 2;
    }

    double a, b;
    int n;
    read(fd, &a, sizeof(double));
    read(fd, &b, sizeof(double));
    read(fd, &n, sizeof(int));

    close(fd);

    double result = calc(a, b, n);

    fd = open("./fifo", O_WRONLY);
    if (fd == -1) {
        printf("Error while opening fifo in %s", argv[0]);
        return 2;
    }

    write(fd, &result, sizeof(double));
    close(fd);

    return 0;
}
