#include <stdio.h>
#include <mqueue.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MESSAGE_BUFFER_SIZE 2048
#define SERVER_QUEUE_NAME "/simple_server_client"
#define MAX_CLIENTS_COUNT 100

typedef enum {
    INIT,
    IDENTIFIER,
    MESSAGE_TEXT
} MessageType;

typedef struct {
    MessageType type;
    int identifier;
    char text[MESSAGE_BUFFER_SIZE];
} Message;

mqd_t createServerQueue();
mqd_t openClientQueue(const char *queueName);
void handleInitMessage(mqd_t serverQueue, Message *message, mqd_t *clientQueues, int *registeredClients);
void handleMessage(mqd_t *clientQueues, Message *message, int registeredClients);
void handleUnexpectedMessageType(MessageType type);

int main() {
    mqd_t serverQueue = createServerQueue();
    if (serverQueue < 0) {
        perror("mq_open server");
        return 1;
    }

    mqd_t clientQueues[MAX_CLIENTS_COUNT];
    int registeredClients = 0;

    printf("Server started...\n");

    while(1) {
        Message receiveMessage;
        if (mq_receive(serverQueue, (char*)&receiveMessage, sizeof(receiveMessage), NULL) == -1) {
            perror("mq_receive");
            continue;
        }

        switch(receiveMessage.type) {
            case INIT:
                handleInitMessage(serverQueue, &receiveMessage, clientQueues, &registeredClients);
                break;
            case MESSAGE_TEXT:
                handleMessage(clientQueues, &receiveMessage, registeredClients);
                break;
            default:
                handleUnexpectedMessageType(receiveMessage.type);
                break;
        }
    }

    return 0;
}

mqd_t createServerQueue() {
    struct mq_attr attributes = {
            .mq_flags = 0,
            .mq_msgsize = sizeof(Message),
            .mq_maxmsg = 10
    };

    return mq_open(SERVER_QUEUE_NAME, O_RDWR | O_CREAT,  S_IRUSR | S_IWUSR, &attributes);
}

mqd_t openClientQueue(const char *queueName) {
    return mq_open(queueName, O_RDWR, S_IRUSR | S_IWUSR, NULL);
}

void handleInitMessage(mqd_t serverQueue, Message *message, mqd_t *clientQueues, int *registeredClients) {
    clientQueues[*registeredClients] = openClientQueue(message->text);
    if(clientQueues[*registeredClients] < 0) {
        perror("mq_open client");
        return;
    }

    Message sendMessage = {
            .type = IDENTIFIER,
            .identifier = *registeredClients
    };

    mq_send(clientQueues[*registeredClients], (char*)&sendMessage, sizeof(sendMessage), 10);
    (*registeredClients)++;
}

void handleMessage(mqd_t *clientQueues, Message *message, int registeredClients) {
    for (int identifier = 0; identifier < registeredClients; identifier++) {
        if (identifier != message->identifier) {
            mq_send(clientQueues[identifier], (char*)message, sizeof(Message), 10);
        }
    }
}

void handleUnexpectedMessageType(MessageType type) {
    printf("Unexpected message type in server queue: %d\n", type);
}
