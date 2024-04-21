#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

double f(double x) {
    return 4/(x*x+1);
}

double calc(double a, double b, double h) {
    double result = 0.0;
    for(int i = 0;  a+i*h< b; i++) {
        result += f(a + i * h);
    }
    return result*h;
}

int main(int argc, char** argv) {
    if(argc < 3) {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    //initialize two integer values a and b

    double h=atof(argv[1]);
    int n = atoi(argv[2]);
    int swf[2];
    int rwf[2];
    pipe(swf);
    pipe(rwf);

    for(int i = 0; i < n; i++) {
        pid_t pid = fork();
        if(pid == -1) {
            perror("Error while creating fork!!");
            return 2;
        } else if(pid == 0) { //child
            close(swf[1]);
            close(rwf[0]);
            double a, b;
            read(swf[0], &a, sizeof(double));
            read(swf[0], &b, sizeof(double));
            read(swf[0], &h, sizeof(double));
            double result = calc(a, b, h);
            write(rwf[1], &result, sizeof(double));
            return 0;

        } else {
            double a = (double)i / n;
            double b = (double)(i + 1) / n;
            write(swf[1], &a, sizeof(double));
            write(swf[1], &b, sizeof(double));
            write(swf[1], &h, sizeof(double));
        }
    }

    close(swf[1]);
    close(rwf[1]);

    double total_result = 0.0;
    for(int i = 0; i < n; i++) {
        double result;
        read(rwf[0], &result, sizeof(double));
        total_result += result;
    }
    close(rwf[0]);

    printf("%f\n", total_result);

    // Waiting for all child processes to finish
    for(int i = 0; i < n; i++) {
        wait(NULL);
    }
    return 0;
}
