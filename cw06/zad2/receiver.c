#include <stdio.h>
#include <unistd.h>
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

int main() {
    char input_fifo_path[] = "./input_fifo";
    char output_fifo_path[] = "./output_fifo";

    printf("Opened receiver\n");

    int ifd = open(input_fifo_path, O_RDONLY);
    int ofd = open(output_fifo_path, O_WRONLY);

    double a, b;
    read(ifd, &a, sizeof(double));
    read(ifd, &b, sizeof(double));
    close(ifd);

    double result = calc(a, b, 100000000);
    printf("Calculated result, returning to sender...\n");
    write(ofd, &result, sizeof(double));
    close(ofd);

    return 0;
}
