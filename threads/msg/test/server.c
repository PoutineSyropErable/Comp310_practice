#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <time.h>

#define MSGSIZE 256
#define KEY 1234

// Enum for procedure IDs
enum ProcID {
    GET_TIME = 1,
    SET_TIME = 2
};

// Message structure
struct msgbuf {
    long mtype;           // Message type (client PID)
    int proc_num;         // Procedure number
    char mtext[MSGSIZE];  // Message data (arguments or result)
};

// Function to get the current system time
void get_time(char *buffer) {
    time_t now = time(NULL);
    struct tm *tm_info = localtime(&now);
    strftime(buffer, MSGSIZE, "Current time is: %Y-%m-%d %H:%M:%S", tm_info);
}

// Function to set time (just echoes on the server side)
void set_time(const char *time_str) {
    printf("The time is now: %s\n", time_str);
}

int main() {
    int msgid;
    struct msgbuf msg;

    // Create or get the message queue
    msgid = msgget(KEY, IPC_CREAT | 0666);
    if (msgid == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Server is running...\n");

    while (1) {
        // Receive a message from any client
        if (msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        printf("Received from client %ld: proc_num=%d, arg=%s\n", msg.mtype, msg.proc_num, msg.mtext);

        // Process the request based on proc_num
        switch (msg.proc_num) {
            case GET_TIME: { // Get time
                char response[MSGSIZE];
                get_time(response);

                // Send the response back to the client
                msg.mtype = msg.mtype;  // Client's PID
                strncpy(msg.mtext, response, MSGSIZE);

                if (msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0) == -1) {
                    perror("msgsnd");
                    exit(EXIT_FAILURE);
                }
                printf("Response sent to client %ld: %s\n", msg.mtype, response);
                break;
            }
            case SET_TIME: { // Set time
                set_time(msg.mtext);  // Echo the time string on the server
                // No response sent back to the client
                break;
            }
            default:
                printf("Invalid procedure number: %d\n", msg.proc_num);
        }
    }

    // Clean up (optional)
    // msgctl(msgid, IPC_RMID, NULL);

    return 0;
}
