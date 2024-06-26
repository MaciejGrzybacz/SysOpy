#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define ALIVE_INTERVAL 10

typedef struct {
    struct sockaddr_in address;
    char id[BUFFER_SIZE];
    int alive;
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_message_to_client(const char *message, client_t *client, int server_fd) {
    if (sendto(server_fd, message, strlen(message), 0, (struct sockaddr *)&client->address, sizeof(client->address)) < 0) {
        perror("sendto");
        exit(EXIT_FAILURE);
    }
}

void broadcast_message(const char *message, client_t *exclude_client, int server_fd) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] && clients[i] != exclude_client) {
            send_message_to_client(message, clients[i], server_fd);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_client_list(client_t *client, int server_fd) {
    pthread_mutex_lock(&clients_mutex);

    char list[BUFFER_SIZE] = "Active clients: ";
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            strcat(list, clients[i]->id);
            strcat(list, " ");
        }
    }

    pthread_mutex_unlock(&clients_mutex);

    send_message_to_client(list, client, server_fd);
}

void send_private_message(const char *message, const char *recipient_id, client_t *sender, int server_fd) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] && strcmp(clients[i]->id, recipient_id) == 0) {
            send_message_to_client(message, clients[i], server_fd);
            break;
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void remove_client(client_t *client) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] == client) {
            clients[i] = NULL;
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
    free(client);
}

void handle_client(int server_fd) {
    char buffer[BUFFER_SIZE];
    struct sockaddr_in client_addr;
    socklen_t addr_len = sizeof(client_addr);

    while (1) {
        int bytes_received = recvfrom(server_fd, buffer, BUFFER_SIZE, 0, (struct sockaddr *)&client_addr, &addr_len);
        if (bytes_received <= 0) {
            continue;
        }

        buffer[bytes_received] = '\0';

        pthread_mutex_lock(&clients_mutex);
        client_t *client = NULL;
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i] && clients[i]->address.sin_addr.s_addr == client_addr.sin_addr.s_addr &&
                clients[i]->address.sin_port == client_addr.sin_port) {
                client = clients[i];
                break;
            }
        }

        if (!client) {
            for (int i = 0; i < MAX_CLIENTS; ++i) {
                if (!clients[i]) {
                    client = (client_t *)malloc(sizeof(client_t));
                    client->address = client_addr;
                    client->alive = 1;
                    sscanf(buffer, "%s", client->id);
                    clients[i] = client;
                    break;
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if (!client) {
            continue;
        }

        printf("Received message from %s: %s\n", client->id, buffer);

        if (strncmp(buffer, "LIST", 4) == 0) {
            send_client_list(client, server_fd);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            char message[BUFFER_SIZE];
            snprintf(message, BUFFER_SIZE, "[PUBLIC][%.20s]: %.900s", client->id, buffer + 5);
            broadcast_message(message, client, server_fd);
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            char recipient_id[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", recipient_id);
            char *message_content = buffer + 5 + strlen(recipient_id) + 1;
            char message[BUFFER_SIZE];
            snprintf(message, BUFFER_SIZE, "[PRIVATE][%.20s]: %.900s", client->id, message_content);
            send_private_message(message, recipient_id, client, server_fd);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            remove_client(client);
        } else if (strncmp(buffer, "ALIVE", 5) == 0) {
            client->alive = 1;
        }
    }
}

void *alive_checker(void *arg) {
    int server_fd = *(int *)arg;
    while (1) {
        sleep(ALIVE_INTERVAL);

        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (clients[i]) {
                if (clients[i]->alive == 0) {
                    printf("Client %s is not responding. Removing from list.\n", clients[i]->id);
                    remove_client(clients[i]);
                } else {
                    clients[i]->alive = 0;
                    send_message_to_client("ALIVE", clients[i], server_fd);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    int server_fd;
    struct sockaddr_in server_addr;

    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return -1;
    }

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);


    if ((server_fd = socket(AF_INET, SOCK_DGRAM, 0)) == 0) {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }


    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = inet_addr(server_ip);
    server_addr.sin_port = htons(server_port);

    if (bind(server_fd, (struct sockaddr *) &server_addr, sizeof(server_addr)) < 0) {
        perror("bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server started on %s:%d\n", server_ip, server_port);

    pthread_t alive_thread;
    if (pthread_create(&alive_thread, NULL, alive_checker, (void *)&server_fd) != 0) {
        perror("pthread_create failed");
        return -1;
    }

    handle_client(server_fd);

    close(server_fd);
    return 0;
}
