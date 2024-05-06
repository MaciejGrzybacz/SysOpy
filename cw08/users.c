//
// Created by maciejgrzybacz on 06.05.24.
//
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/wait.h>
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

void generate_random_string(char* buffer, int length) {
    for(int i = 0; i < length; i++) {
        buffer[i] = 'a' + rand() % 26;
    }
    buffer[length] = '\0';
}

int main(int argc, char** argv) {
    if(argc < 2) {
        printf("Usage: %s <number_of_users>\n", argv[0]);
        return -1;
    }

    long number_of_users = strtol(argv[1], NULL, 10);

    int memory_fd = shm_open(SHARED_MEMORY_DESCRIPTOR_NAME, O_RDWR, S_IRUSR | S_IWUSR);
    if(memory_fd < 0) {
        printf("Failed to open shared memory\n");
        return -1;
    }

    memory_map_t* memory_map = mmap(NULL, sizeof(memory_map_t), PROT_READ | PROT_WRITE, MAP_SHARED, memory_fd, 0);
    if (memory_map == MAP_FAILED) {
        printf("Failed to map shared memory\n");
        return -1;
    }

    char user_buffer[MAX_PRINTER_BUFFER_SIZE] = {0};

    for (int sig = 1; sig < SIGRTMAX; sig++) {
        signal(sig, SIGNAL_handler);
    }

    for (int i = 0; i < number_of_users; i++){
        pid_t user_pid = fork();
        if (user_pid < 0) {
            printf("Failed to fork\n");
            return -1;
        }
        else if(user_pid == 0) {
            while(!should_close) {
                generate_random_string(user_buffer, 10);
                int printer_index = -1;
                for (int j = 0; j < memory_map->number_of_printers; j++) {
                    int val;
                    sem_getvalue(&memory_map->printers[j].printer_semaphore, &val);
                    if(val > 0) {
                        printer_index = j;
                        break;
                    }
                }

                if(printer_index == -1)
                    printer_index = rand() % memory_map->number_of_printers;

                if(sem_wait(&memory_map->printers[printer_index].printer_semaphore) < 0) {
                    printf("Failed to wait on semaphore\n");
                    return -1;
                }

                memcpy(memory_map->printers[printer_index].printer_buffer, user_buffer, MAX_PRINTER_BUFFER_SIZE);
                memory_map->printers[printer_index].printer_buffer_size = strlen(user_buffer);

                memory_map->printers[printer_index].printer_state = PRINTING;

                printf("User %d is printing on printer %d\n", i, printer_index);
                fflush(stdout);

                sleep(rand() % 3 + 1);
            }
            exit(0);
        }
    }

    while(wait(NULL) > 0) {};
    munmap(memory_map, sizeof(memory_map_t));
}