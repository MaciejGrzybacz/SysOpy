//
// Created by maciejgrzybacz on 06.05.24.
//
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <string.h>
#include <signal.h>
#include <stdbool.h>
#include <semaphore.h>

#define SHARED_MEMORY_DESCRIPTOR_NAME "printer_system_shared_memory"
#define MAX_PRINTERS 256
#define MAX_PRINTER_BUFFER_SIZE 256
#define MAX_SEMAPHORE_NAME 40

typedef enum {
    WAITING = 0,
    PRINTING = 1
} printer_state_t;

typedef struct {
    sem_t printer_semaphore;
    char printer_buffer[MAX_PRINTER_BUFFER_SIZE];
    size_t printer_buffer_size;
    printer_state_t printer_state;
} printer_t;

typedef struct {
    printer_t printers[MAX_PRINTERS];
    int number_of_printers;
} memory_map_t;

volatile bool should_close = false;

void SIGNAL_handler(int signum) {
    should_close = true;
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number_of_printers>\n", argv[0]);
        return -1;
    }
    long number_of_printers = strtol(argv[1], NULL, 10);

    if (number_of_printers > MAX_PRINTERS) {
        printf("Too many printers!\n");
        return -1;
    } else if(number_of_printers < 1) {
        printf("Number of printers must be greater than 0!\n");
        return -1;
    }

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR);
    if(memory_fd < 0) {
        printf("shm_open failed!\n");
        return -1;
    }

    if (ftruncate(memory_fd, sizeof(memory_map_t)) < 0) {
        printf("ftruncate failed!\n");
        return -1;
    }


    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED){
        printf("mmap failed!\n");
        return -1;
    }

    memset(memory_map, 0, sizeof(memory_map_t));

    memory_map->number_of_printers = number_of_printers;

    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }

    for (int i = 0; i < number_of_printers; i++){
        sem_init(&memory_map->printers[i].printer_semaphore, 1, 1);

        pid_t printer_pid = fork();
        if(printer_pid < 0) {
            printf("Fork failed!\n");
            return -1;
        }
        else if(printer_pid == 0) {
            while(!should_close) {
                if (memory_map->printers[i].printer_state == PRINTING) {

                    for (int j = 0; j < memory_map->printers[i].printer_buffer_size; j++) {
                        printf("%c", memory_map->printers[i].printer_buffer[j]);
                        sleep(1);
                    }

                    printf("\n");
                    fflush(stdout);

                    memory_map->printers[i].printer_state = WAITING;

                    sem_post(&memory_map->printers[i].printer_semaphore);
                }
            }
            exit(0);
        }
    }

    while(wait(NULL) > 0) {}

    for (int i = 0; i < number_of_printers; i++)
        sem_destroy(&memory_map->printers[i].printer_semaphore);

    munmap(memory_map, sizeof(memory_map_t));
    close(memory_fd);
    shm_unlink(SHARED_MEMORY_DESCRIPTOR_NAME);
}