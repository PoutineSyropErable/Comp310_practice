#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
  int server_fd, client_fd;
  struct sockaddr_in server_addr, client_addr;
  socklen_t addr_len = sizeof(client_addr);
  char buffer[BUFFER_SIZE];

  // Create a socket
  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY; // Listen on all network interfaces
  server_addr.sin_port = htons(PORT);

  // Bind to the port
  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Bind failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Listen for incoming connections
  if (listen(server_fd, 5) < 0) {
    perror("Listen failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server is waiting for connections on port %d...\n", PORT);

  // Accept a connection
  client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &addr_len);
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
  printf("Server shut down.\n");

  return 0;
}
