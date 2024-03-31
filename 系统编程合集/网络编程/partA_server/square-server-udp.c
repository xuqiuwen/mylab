#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <time.h>
#include <unistd.h>
#define SERVER_PORT 12345
#define BUF_SIZE 1024

int main() {
    int server_sock_fd;
    struct sockaddr_in server_addr, client_addr;
    char buffer[BUF_SIZE];

    server_sock_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_sock_fd == -1) {
        perror("Socket creation error");
        exit(EXIT_FAILURE);
    }

    // Enable address reuse option
    int reuseaddr = 1;
    if (setsockopt(server_sock_fd, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(server_sock_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1) {
        perror("Bind error");
        exit(EXIT_FAILURE);
    }

    printf("Server is listening on port %d...\n", SERVER_PORT);

    while (1) {
        memset(buffer, 0, sizeof(buffer)); // Clear the buffer

        socklen_t client_addr_len = sizeof(client_addr);
        ssize_t received_bytes = recvfrom(server_sock_fd, buffer, BUF_SIZE, 0, (struct sockaddr *)&client_addr, &client_addr_len);
        if (received_bytes == -1) {
            perror("recvfrom error");
            break;
        }

        char client_ip[INET_ADDRSTRLEN];
        inet_ntop(AF_INET, &(client_addr.sin_addr), client_ip, INET_ADDRSTRLEN);
        printf("Recieve from %s:%d\n", client_ip, ntohs(client_addr.sin_port));

        buffer[received_bytes] = '\0';

        if (buffer[0] == 'd') {
            int n = atoi(buffer + 2);
            sprintf(buffer, "%d", n * n);
        } else if (buffer[0] == 'f') {
            double n = atof(buffer + 2);
            sprintf(buffer, "%lf", n * n);
        } else {
            strcpy(buffer, "Invailed input");
        }
        if (sendto(server_sock_fd, buffer, sizeof(buffer), 0, (struct sockaddr *)&client_addr, client_addr_len) == -1) {
            perror("Sendto error");
            break;
        }
    }

    close(server_sock_fd);

    return 0;
}
