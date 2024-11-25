#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MSGSIZE 256
#define KEY 1234

// Enum for procedure IDs
enum ProcID { GET_TIME = 1,
	          SET_TIME = 2 };

// Message structure
struct msgbuf {
	long pid;            // Message type (client PID)
	int proc_num;        // Procedure number
	char mtext[MSGSIZE]; // Message data
};

// Function prototypes
char* to_server_get_time();
void to_server_set_time(const char* time_str);

int main() {
	/* Explanation:
	 * This client communicates with the server using System V message queues.
	 * It provides two main functionalities:
	 * 1. Get the current time from the server.
	 * 2. Set a specified time (simulated, does not affect system time).
	 *
	 * Communication steps:
	 * - The client sends a request with its process ID (pid) and procedure number.
	 * - For GET_TIME, the server responds with the current time.
	 * - For SET_TIME, the server logs the time string and does not respond.
	 */

	// Call the "get time" method
	char* response = to_server_get_time();
	printf("Server responded: %s\n", response);
	free(response); // Free dynamically allocated memory

	// Call the "set time" method
	const char* time_to_set = "2024-11-24 16:30:00";
	to_server_set_time(time_to_set);

	return 0;
}

// Method: to_server_get_time
char* to_server_get_time() {
	/* List of task:
	 * 1. Get the message ID

	 * 2. Create a single argument containing the pid, the procedure, and the
	 * message content/argument.

	 * 3. Send the message to the server.

	 * 4. Wait and receive a message from the server (The output).
	 */

	// Step 1: Get the message ID
	int msgid = msgget(KEY, 0666);
	if (msgid == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	// Step 2: Create a message containing the pid, procedure, and content
	struct msgbuf msg;
	pid_t pid = getpid();
	msg.pid = pid;           // Use PID for unique message type
	msg.proc_num = GET_TIME; // Procedure ID for "get time"
	strcpy(msg.mtext, "");   // No arguments needed

	// Step 3: Send the message to the server
	int message_send_ret = msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
	if (message_send_ret == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}

	// Step 4: Wait and receive a message from the server
	int message_recieve_ret = msgrcv(msgid, &msg, sizeof(msg) - sizeof(long), pid, 0);
	if (message_recieve_ret == -1) {
		perror("msgrcv");
		exit(EXIT_FAILURE);
	}

	// Allocate memory for the response and copy it
	char* response = malloc(MSGSIZE);
	if (!response) {
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	strncpy(response, msg.mtext, MSGSIZE);
	return response; // Return the server response
}

// Method: to_server_set_time
void to_server_set_time(const char* time_str) {
	/* List of task:
	 * 1. Get the message ID
	 * 2. Create a single argument containing the pid, the procedure, and the
	 * message content/argument.
	 * 3. Send the message to the server.
	 */

	// Step 1: Get the message ID
	int msgid = msgget(KEY, 0666);
	if (msgid == -1) {
		perror("msgget");
		exit(EXIT_FAILURE);
	}

	// Step 2: Create a message containing the pid, procedure, and content
	struct msgbuf msg;
	pid_t pid = getpid();
	msg.pid = pid;                         // Use PID for unique message type
	msg.proc_num = SET_TIME;               // Procedure ID for "set time"
	strncpy(msg.mtext, time_str, MSGSIZE); // Pass the time string

	// Step 3: Send the message to the server
	int message_send_ret = msgsnd(msgid, &msg, sizeof(msg) - sizeof(long), 0);
	if (message_send_ret == -1) {
		perror("msgsnd");
		exit(EXIT_FAILURE);
	}

	// No response expected from the server
}
