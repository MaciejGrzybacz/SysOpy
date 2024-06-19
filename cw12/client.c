#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUFFER_SIZE 1024

int client_connection = 1;
int sock = 0;
struct sockaddr_in serv_addr;
char client_id[BUFFER_SIZE];

void handle_exit() {
    if (client_connection) {
        sendto(sock, "STOP", strlen("STOP"), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        close(sock);
        client_connection = 0;
    }
    printf("Client exiting...\n");
    exit(0);
}

void *receive_messages(void *arg) {
    char buffer[BUFFER_SIZE];
    socklen_t addr_len = sizeof(serv_addr);
    while (1) {
        int bytes_received = recvfrom(sock, buffer, BUFFER_SIZE, 0, (struct sockaddr *) &serv_addr, &addr_len);
        if (bytes_received <= 0) {
            printf("Server disconnected.\n");
            handle_exit();
            break;
        }
        buffer[bytes_received] = '\0';

        if (strncmp(buffer, "ALIVE", 5) == 0) {
            sendto(sock, "ALIVE", strlen("ALIVE"), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
        } else {
            printf("%s\n", buffer);
        }
    }
    return NULL;
}

int main(int argc, char const *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <client_id> <server_ip> <server_port>\n", argv[0]);
        return -1;
    }

    pthread_t thread;

    strcpy(client_id, argv[1]);
    const char *server_ip = argv[2];
    int server_port = atoi(argv[3]);

    // Create socket
    if ((sock = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        perror("Socket creation error");
        return -1;
    }

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);

    // Convert IPv4 and IPv6 addresses from text to binary form
    if (inet_pton(AF_INET, server_ip, &serv_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        return -1;
    }

    // Send client ID to server
    sendto(sock, client_id, strlen(client_id), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));

    // Handle Ctrl + C Exit
    signal(SIGINT, handle_exit);

    // Create thread to receive messages
    if (pthread_create(&thread, NULL, receive_messages, NULL) != 0) {
        perror("pthread_create failed");
        return -1;
    }

    // Main loop to send messages
    char buffer[BUFFER_SIZE];
    while (client_connection) {
        fgets(buffer, BUFFER_SIZE, stdin);
        if (strncmp(buffer, "STOP", 4) == 0) {
            handle_exit();
        }
        sendto(sock, buffer, strlen(buffer), 0, (struct sockaddr *) &serv_addr, sizeof(serv_addr));
    }

    // Wait for the receive thread to finish
    pthread_join(thread, NULL);

    return 0;
}
