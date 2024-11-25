#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_PATH "/tmp/simple_rpc_socket"
#define BUFFER_SIZE 1024

int main() {
    int server_fd, client_fd;
    struct sockaddr_un server_addr;
    char buffer[BUFFER_SIZE];

    // Create a socket
    server_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (server_fd < 0) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Bind to a socket path
    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sun_family = AF_UNIX;
    strncpy(server_addr.sun_path, SOCKET_PATH, sizeof(server_addr.sun_path) - 1);
    unlink(SOCKET_PATH); // Ensure the path is clear

    if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("Bind failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    // Listen for a connection
    if (listen(server_fd, 1) < 0) {
        perror("Listen failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Server is waiting for connections...\n");

    // Accept a connection
    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd < 0) {
        perror("Accept failed");
        close(server_fd);
        exit(EXIT_FAILURE);
    }

    printf("Client connected. Waiting for requests...\n");

    // Receive a request
    memset(buffer, 0, BUFFER_SIZE);
    if (recv(client_fd, buffer, BUFFER_SIZE, 0) > 0) {
        printf("Received request: %s\n", buffer);

        // Execute the `ls` command
        FILE *ls_output = popen("ls", "r");
        if (!ls_output) {
            perror("Failed to execute ls");
            close(client_fd);
            close(server_fd);
            exit(EXIT_FAILURE);
        }

        // Read the output of `ls` and send it back to the client
        while (fgets(buffer, BUFFER_SIZE, ls_output) != NULL) {
            send(client_fd, buffer, strlen(buffer), 0);
        }

        pclose(ls_output);
    }

    // Clean up
    close(client_fd);
    close(server_fd);
    unlink(SOCKET_PATH);
    printf("Server shut down.\n");

    return 0;
}
