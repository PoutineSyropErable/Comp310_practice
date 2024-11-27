#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#define SERVER_IP "10.0.0.9" // Replace with the server's IP address
#define PORT 12345
#define BUFFER_SIZE 1024

int main() {
  int client_fd;
  struct sockaddr_in server_addr;
  char buffer[BUFFER_SIZE];

  // Create a socket
  client_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (client_fd < 0) {
    perror("Socket creation failed");
    exit(EXIT_FAILURE);
  }

  // Configure server address
  memset(&server_addr, 0, sizeof(server_addr));
  server_addr.sin_family = AF_INET;
  server_addr.sin_port = htons(PORT);
  if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
    perror("Invalid address/Address not supported");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  // Connect to the server
  if (connect(client_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) <
      0) {
    perror("Connection to server failed");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  printf("Connected to the server. Sending request...\n");

  // Send a simple request
  const char *request = "Run ls";
  if (send(client_fd, request, strlen(request), 0) < 0) {
    perror("Failed to send request");
    close(client_fd);
    exit(EXIT_FAILURE);
  }

  printf("Request sent. Receiving response...\n");

  // Receive and print the response
  while (1) {
    memset(buffer, 0, BUFFER_SIZE);
    int bytes_received = recv(client_fd, buffer, BUFFER_SIZE - 1, 0);
    if (bytes_received <= 0)
      break; // No more data
    printf("%s", buffer);
  }

  // Clean up
  close(client_fd);
  printf("\nClient shut down.\n");

  return 0;
}
