#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdlib.h>

#define MESSAGE_BUFFER_SIZE 2048
#define SERVER_QUEUE_NAME "/simple_server_client"
#define CLIENT_QUEUE_NAME_SIZE 40
#define MAX_CLIENTS_COUNT 40
#define MIN(a, b) (a < b ? a : b)

typedef enum {
    INIT,
    IDENTIFIER,
    MESSAGE_TEXT,
    KILL
} message_type_t;

typedef struct {
    message_type_t type;
    int identifier;
    char text[MESSAGE_BUFFER_SIZE];
} message_t;


mqd_t initialize_queue(const char* name, struct mq_attr* attr, int flags, mode_t mode);
void send_initial_message(mqd_t mq_server_descriptor, const char* queue_name);
void handle_messages(mqd_t mq_client_descriptor, int* pipe_write_end);
void read_and_send_messages(mqd_t mq_server_descriptor, int* pipe_read_end);
int setup_pipes_and_fork(int* pipe_write_end, int* pipe_read_end);

int main() {
    pid_t pid = getpid();
    char queue_name[CLIENT_QUEUE_NAME_SIZE] = {0};
    sprintf(queue_name, "/simple_chat_client_queue_%d", pid);

    struct mq_attr attributes = {
            .mq_flags = 0,
            .mq_msgsize = sizeof(message_t),
            .mq_maxmsg = 10
    };

    mqd_t mq_client_descriptor = initialize_queue(queue_name, &attributes, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
    mqd_t mq_server_descriptor = initialize_queue(SERVER_QUEUE_NAME, NULL, O_RDWR, S_IRUSR | S_IWUSR);

    send_initial_message(mq_server_descriptor, queue_name);

    int pipe_end = setup_pipes_and_fork(NULL, NULL);
    if (pipe_end < 0) return 1;  // Error in pipe or fork

    if (getpid() == pid) { // Parent
        read_and_send_messages(mq_server_descriptor, &pipe_end);
    } else { // Child
        handle_messages(mq_client_descriptor, &pipe_end);
    }
}

mqd_t initialize_queue(const char* name, struct mq_attr* attr, int flags, mode_t mode) {
    mqd_t mqd = mq_open(name, flags, mode, attr);
    if (mqd < 0) perror("mq_open");
    return mqd;
}

void send_initial_message(mqd_t mq_server_descriptor, const char* queue_name) {
    message_t message_init = {
            .type = INIT,
            .identifier = -1
    };
    memcpy(message_init.text, queue_name, MIN(CLIENT_QUEUE_NAME_SIZE - 1, strlen(queue_name)));
    if (mq_send(mq_server_descriptor, (char*)&message_init, sizeof(message_init), 10) < 0) {
        perror("mq_send init");
    }
}

void handle_messages(mqd_t mq_client_descriptor, int* pipe_write_end) {
    message_t receive_message;
    while (1) {
        if (mq_receive(mq_client_descriptor, (char*)&receive_message, sizeof(receive_message), NULL) >= 0) {
            switch (receive_message.type) {
                case MESSAGE_TEXT:
                    printf("Received from id: %d message: %s\n", receive_message.identifier, receive_message.text);
                    break;
                case IDENTIFIER:
                    printf("Received identifier from server: %d\n", receive_message.identifier);
                    write(*pipe_write_end, &receive_message.identifier, sizeof(receive_message.identifier));
                    break;
                case KILL:
                    printf("Too many clients, cannot register!");
                    exit(1);
                default:
                    printf("Unknown message type in client queue: %d", receive_message.type);
                    break;
            }
        }
    }
}

void read_and_send_messages(mqd_t mq_server_descriptor, int* pipe_read_end) {
    int identifier;
    read(*pipe_read_end, &identifier, sizeof(identifier));
    char* buffer = NULL;
    while (1) {
        if (scanf("%ms", &buffer) == 1) {
            message_t send_message = {
                    .type = MESSAGE_TEXT,
                    .identifier = identifier
            };
            memcpy(send_message.text, buffer, MIN(MESSAGE_BUFFER_SIZE - 1, strlen(buffer)));
            mq_send(mq_server_descriptor, (char*)&send_message, sizeof(send_message), 10);
            free(buffer);
            buffer = NULL;
        } else {
            perror("scanf input");
        }
    }
}

int setup_pipes_and_fork(int* pipe_write_end, int* pipe_read_end) {
    int to_parent_pipe[2];
    if (pipe(to_parent_pipe) < 0) {
        perror("pipe");
        return -1;
    }
    pid_t pid = fork();
    if (pid < 0) {
        perror("fork listener");
        return -1;
    } else if (pid == 0) { // Child
        close(to_parent_pipe[0]);
        return to_parent_pipe[1];  // Return the write end of the pipe
    } else { // Parent
        close(to_parent_pipe[1]);
        return to_parent_pipe[0];  // Return the read end of the pipe
    }
}

