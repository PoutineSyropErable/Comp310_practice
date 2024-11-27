#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSGSIZE 256
#define KEY 1234

// Enum for procedure IDs
enum ProcID {
    GET_TIME = 1,
    SET_TIME = 2
};

// Message structure
struct msgbuf {
    long mtype;           // Message type
    int proc_num;         // Procedure number
    char mtext[MSGSIZE];  // Message data
};

// Function prototypes
void to_server_get_time(char *response);
void to_server_set_time(const char *time_str);

int main() {
    char response[MSGSIZE];

    // Call the "get time" method
    to_server_get_time(response);
    printf("Server responded: %s\n", response);

    // Call the "set time" method
    const char *time_to_set = "2024-11-24 16:30:00";
    to_server_set_time(time_to_set);

    return 0;
}

// Method: to_server_get_time
void to_server_get_time(char *response) {
    int msgid;
    struct msgbuf msg;

    // Get the message queue
    msgid = msgget(KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Prepare a message to request the time
    pid_t pid = getpid();
    msg.mtype = pid;         // Use PID for unique message type
    msg.proc_num = GET_TIME; // Procedure ID for "get time"
    strcpy(msg.mtext, "");   // No arguments needed

    // Send the request to the server
    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    // Wait for a response from the server
    if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), pid, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    // Copy the response to the provided buffer
    strncpy(response, msg.mtext, MSGSIZE);
}

// Method: to_server_set_time
void to_server_set_time(const char *time_str) {
    int msgid;
    struct msgbuf msg;

    // Get the message queue
    msgid = msgget(KEY, 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Prepare a message to set the time
    pid_t pid = getpid();
    msg.mtype = pid;         // Use PID for unique message type
    msg.proc_num = SET_TIME; // Procedure ID for "set time"
    strncpy(msg.mtext, time_str, MSGSIZE); // Pass the time string

    // Send the request to the server
    if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    // No response expected from the server
}
