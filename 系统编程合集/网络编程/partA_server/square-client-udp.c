#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <unistd.h>

#define TODO()                                                                \
    do {                                                                      \
        extern int printf(char *, ...);                                       \
        printf("Add your code here: file %s, line %d\n", __FILE__, __LINE__); \
    } while (0)

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 12345
#define BUF_SIZE 1024
#define MAX_RETRIES 3
#define TIMEOUT_SECONDS 2

int main() {
    int client_sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUF_SIZE];

    client_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (client_sock_fd == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    struct timeval timeout;
    timeout.tv_sec = TIMEOUT_SECONDS;
    timeout.tv_usec = 0;
    setsockopt(client_sock_fd, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server_addr.sin_addr) <= 0) {
        perror("Invalid address/Address not supported");
        exit(EXIT_FAILURE);
    }

    printf("Enter 'bye' to exit.\n");

    while (1) {
        printf("Enter a message: ");
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        int retries = 0;
        ssize_t received_bytes;
        while (retries < MAX_RETRIES) { // 超时重传
            if (sendto(client_sock_fd, buffer, strlen(buffer), 0, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
                perror("Sendto error");
                break;
            }

            received_bytes = recvfrom(client_sock_fd, buffer, BUF_SIZE - 1, 0, NULL, NULL);
            if (received_bytes > 0) {
                break;
            }

            printf("No response from server, retrying...\n");
            retries++;
        }

        if (retries == MAX_RETRIES) {
            printf("Maximum retries reached. Exiting...\n");
            break;
        }

        if (received_bytes <= 0) {
            perror("recvfrom error");
            break;
        }

        if (strcmp(buffer, "bye") == 0) {
            printf("Exiting...\n");
            break;
        }

        buffer[received_bytes] = '\0';
        printf("Server response: %s\n", buffer);
    }

    close(client_sock_fd);

    return 0;
}
