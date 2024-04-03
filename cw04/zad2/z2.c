#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int global = 0;

int main(int argc, char *argv[]) {
    if (argc != 2) {
        return 1;
    }

    int local = 0;

    printf("Program name: %s\n", argv[0]);

    pid_t pid = fork();
    if (pid == -1) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        printf("child process\n");

        global++;
        local++;

        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);

        int exec_return = execl("/bin/ls", "ls", argv[1], NULL);
        if (exec_return == -1) {
            exit(1);
        }
    } else {
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), pid);

        int status;
        waitpid(pid, &status, 0);

        if (WIFEXITED(status)) {
            printf("Child exit code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child process did not exit normally\n");
        }

        printf("Parent's local = %d, parent's global = %d\n", local, global);

        return 0;
    }
}
