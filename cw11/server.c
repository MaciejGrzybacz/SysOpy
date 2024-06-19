// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <pthread.h>

#define MAX_CLIENTS 100
#define BUFFER_SIZE 1024
#define ALIVE_INTERVAL 10

typedef struct {
    int socket;
    char id[BUFFER_SIZE];
    int alive;
} client_t;

client_t *clients[MAX_CLIENTS];
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

void send_message_to_client(const char *message, client_t *client) {
    if (send(client->socket, message, strlen(message), 0) < 0) {
        perror("send");
    }
}

void broadcast_message(const char *message, client_t *exclude_client) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] && clients[i] != exclude_client) {
            send_message_to_client(message, clients[i]);
        }
    }

    pthread_mutex_unlock(&clients_mutex);
}

void send_client_list(client_t *client) {
    pthread_mutex_lock(&clients_mutex);

    char list[BUFFER_SIZE] = "Active clients: ";
    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i]) {
            strcat(list, clients[i]->id);
            strcat(list, " ");
        }
    }

    pthread_mutex_unlock(&clients_mutex);

    send_message_to_client(list, client);
}

void send_private_message(const char *message, const char *recipient_id, client_t *sender) {
    pthread_mutex_lock(&clients_mutex);

    for (int i = 0; i < MAX_CLIENTS; ++i) {
        if (clients[i] && strcmp(clients[i]->id, recipient_id) == 0) {
            send_message_to_client(message, clients[i]);
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
    close(client->socket);
    free(client);
}

void handle_client(client_t *client) {
    char buffer[BUFFER_SIZE];
    while (1) {
        int bytes_received = recv(client->socket, buffer, BUFFER_SIZE, 0);
        if (bytes_received <= 0) {
            remove_client(client);
            break;
        }

        buffer[bytes_received] = '\0';

        printf("Received message from %s: %s\n", client->id, buffer);

        if (strncmp(buffer, "LIST", 4) == 0) {
            send_client_list(client);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            char message[BUFFER_SIZE];
            snprintf(message, BUFFER_SIZE, "[PUBLIC][%s]: %s", client->id, buffer + 5);
            broadcast_message(message, client);
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            char recipient_id[BUFFER_SIZE];
            sscanf(buffer + 5, "%s", recipient_id);
            char *message_content = buffer + 5 + strlen(recipient_id) + 1;
            char message[BUFFER_SIZE];
            snprintf(message, BUFFER_SIZE, "[PRIVATE][%s]: %s", client->id, message_content);
            send_private_message(message, recipient_id, client);
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            remove_client(client);
            break;
        } else if (strncmp(buffer, "ALIVE", 5) == 0) {
            client->alive = 1;
        }
    }
}

void *alive_checker() {
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
                    send_message_to_client("ALIVE", clients[i]);
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}

int main(int argc, char const *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <server_ip> <server_port>\n", argv[0]);
        return 1;
    }

    int server_fd, new_socket;
    struct sockaddr_in address;
    int address_length = sizeof(address);

    const char *server_ip = argv[1];
    int server_port = atoi(argv[2]);

    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        perror("socket failed");
        return 1;
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = inet_addr(server_ip);
    address.sin_port = htons(server_port);

    if (bind(server_fd, (struct sockaddr *) &address, sizeof(address)) < 0) {
        perror("bind failed");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 3) < 0) {
        perror("listen failed");
        close(server_fd);
        return 1;
    }

    printf("Server started on %s:%d\n", server_ip, server_port);

    pthread_t alive_thread;
    if (pthread_create(&alive_thread, NULL, alive_checker, NULL) != 0) {
        perror("pthread_create failed");
        return 1;
    }

    while (1) {
        if ((new_socket = accept(server_fd, (struct sockaddr *) &address, (socklen_t *) &address_length)) < 0) {
            perror("accept failed");
            continue;
        }

        pthread_t thread;
        client_t *new_client = (client_t *) malloc(sizeof(client_t));
        new_client->socket = new_socket;
        new_client->alive = 1;
        recv(new_socket, new_client->id, BUFFER_SIZE, 0);

        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; ++i) {
            if (!clients[i]) {
                clients[i] = new_client;
                printf("Client %s joined the server!", new_client->id);
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        if (pthread_create(&thread, NULL, (void *(*)(void *)) handle_client, (void *) new_client) != 0) {
            perror("pthread_create failed");
            continue;
        }

        pthread_detach(thread);
    }

    close(server_fd);
    return 0;
}
