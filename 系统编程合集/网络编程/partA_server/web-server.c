#include <arpa/inet.h>
#include <errno.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUF_SIZE 1024
#define FIELD_SIZE 100

void serve_client(int client_sock, const char *web_root) {
    char request[BUF_SIZE];
    ssize_t bytes_received = read(client_sock, request, sizeof(request) - 1);
    if (bytes_received < 0) {
        perror("read");
        return;
    }
    request[bytes_received] = '\0';

    // Parse the HTTP request, extract the requested file path, and handle the request.
    printf("Received request:\n%s\n", request);

    // sleep(10); // 模拟高延时环境
    char method[16], path[128];
    int scanResult = sscanf(request, "%s %s", method, path);
    if (scanResult != 2) {
        char response[] = "HTTP/1.1 400 Bad Request\r\nContent-Length: 0\r\n\r\n";
        write(client_sock, response, strlen(response));
        close(client_sock);
        return;
    }

    char fullpath[256];
    snprintf(fullpath, sizeof(fullpath), "%s%s", web_root, path);

    if (strcmp(path, "/favicon.ico") == 0) {
        char response[] = "HTTP/1.1 204 No Content\r\n\r\n";
        write(client_sock, response, strlen(response));
        close(client_sock);
        return;
    }

    char response[FIELD_SIZE] = "";
    char length[FIELD_SIZE] = "Content-Length: ";
    char type[FIELD_SIZE] = "Content-type: text/html\r\n\r\n";
    char content[BUF_SIZE] = "";
    FILE *fp = fopen(fullpath, "r");
    // Handle GET requests for serving files.
    if (strcasecmp(method, "GET") == 0) {

        if (fp == NULL) {
            if (errno == ENOENT) {
                strcpy(response, "HTTP/1.1 404 Not Found\r\n");
                strcpy(content, "<html><body><h1>404 Not Found</h1></body></html>");
                sprintf(length + strlen(length), "%ld\r\n", strlen(content));
                strcat(response, length);
                strcat(response, type);
                write(client_sock, response, strlen(response));
                write(client_sock, content, strlen(content));
            } else {
                char response[] = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
                write(client_sock, response, strlen(response));
            }
        } else {
            strcpy(response, "HTTP/1.1 200 OK\r\n");
            fseek(fp, 0, SEEK_END);
            int size = ftell(fp);
            sprintf(length + strlen(length), "%d\r\n", size);
            strcat(response, length);
            strcat(response, type);
            write(client_sock, response, strlen(response));
            rewind(fp);
            int read_size;

            while (!feof(fp)) {
                read_size = fread(content, sizeof(char), sizeof(content), fp);
                write(client_sock, content, read_size);
            }
        }
    }
    // Handle DELETE requests for deleting files.

    else if (strcasecmp(method, "DELETE") == 0) {

        if (remove(fullpath) == 0) {
            strcpy(response, "HTTP/1.1 200 OK\r\n");
            strcpy(content, "<html><body><h1>File deleted.</h1></body></html>");
            sprintf(length + strlen(length), "%ld\r\n", strlen(content));
            strcat(response, length);
            strcat(response, type);
            write(client_sock, response, strlen(response));
            write(client_sock, content, strlen(content));
        } else {
            if (errno == ENOENT) {
                strcpy(response, "HTTP/1.1 404 Not Found\r\n");
                strcpy(content, "<html><body><h1>404 Not Found</h1></body></html>");
                sprintf(length + strlen(length), "%ld\r\n", strlen(content));
                strcat(response, length);
                strcat(response, type);
                write(client_sock, response, strlen(response));
                write(client_sock, content, strlen(content));
            } else {
                char response[] = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 0\r\n\r\n";
                write(client_sock, response, strlen(response));
            }
        }

    } else {
        char response[] = "HTTP/1.1 405 Method Not Allowed\r\nContent-Length: 0\r\n\r\n";
        write(client_sock, response, strlen(response));
    }
    close(client_sock);
}

int main() {
    char web_root[256];
    strcpy(web_root, getenv("HOME"));
    strcat(web_root, "/mywebsite");

    int server_sock, client_sock;
    struct sockaddr_in server_addr, client_addr;
    socklen_t client_addr_len = sizeof(client_addr);

    server_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (server_sock < 0) {
        perror("socket");
        exit(EXIT_FAILURE);
    }

    int reuseaddr = 1;
    if (setsockopt(server_sock, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(int)) == -1) {
        perror("Setsockopt error");
        exit(EXIT_FAILURE);
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(SERVER_PORT);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    if (bind(server_sock, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        perror("bind");
        exit(EXIT_FAILURE);
    }

    if (listen(server_sock, 5) < 0) {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    printf("Server listening on port %d...\n", SERVER_PORT);

    signal(SIGCHLD, SIG_IGN); // 让系统回收僵尸进程
    while (1) {
        client_sock = accept(server_sock, (struct sockaddr *)&client_addr, &client_addr_len);
        if (client_sock < 0) {
            perror("accept");
            continue;
        }

        if (fork() == 0) {
            serve_client(client_sock, web_root);
            exit(0);
        }
    }

    close(server_sock);
    return 0;
}
