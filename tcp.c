#include <arpa/inet.h>
#include <complex.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define PORT_NUMBER 5173
#define BUFFER_SIZE 1024

int main() {
  int server_fd, client_fd; // File Descriptors
  struct sockaddr_in server_addr, client_addr;
  char buffer[BUFFER_SIZE];
  socklen_t client_addr_len = sizeof(client_addr);

  printf("TCP Server\n");

  server_fd = socket(AF_INET, SOCK_STREAM, 0);
  if (server_fd == -1) {
    perror("Socket failed");
    exit(EXIT_FAILURE);
  }

  server_addr.sin_family = AF_INET;
  server_addr.sin_addr.s_addr = INADDR_ANY;
  server_addr.sin_port = htons(PORT_NUMBER);

  if (bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) ==
      -1) {
    perror("Bind Failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  if (listen(server_fd, 5) == -1) {
    perror("Listen Failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Server is listening on port %d\n", PORT_NUMBER);

  client_fd =
      accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
  if (client_fd == -1) {
    perror("accept failed");
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  printf("Connection accepted from %s:%d\n", inet_ntoa(client_addr.sin_addr),
         ntohs(client_addr.sin_port));

  int bytes_recieved = recv(client_fd, buffer, BUFFER_SIZE, 0);
  if (bytes_recieved == -1) {
    perror("Recv failed");
    close(client_fd);
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  buffer[bytes_recieved] = '\0';
  printf("Received message: %s\n", buffer);

  const char *response = "Hello from the server!";
  if (send(client_fd, response, strlen(response), 0) == -1) {
    perror("send failed");
    close(client_fd);
    close(server_fd);
    exit(EXIT_FAILURE);
  }

  // Close the sockets
  close(client_fd);
  close(server_fd);

  return 0;
}
