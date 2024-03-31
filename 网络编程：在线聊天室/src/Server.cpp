#include "Epoll.h"
#include "Message.h"
#include "MessageQueue.h"
#include <cstring>
#include <iostream>
#include <netinet/in.h>
#include <queue>
#include <sys/epoll.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#include <unordered_map>
#define EPOLL_SIZE 1000
#define BUF_SIZE 1000
using namespace std;
class Server {
  private:
    int listen_sock;                      // 监听套接字
    struct sockaddr_in serv_addr;         // 服务器地址(协议+IP+端口)
    Epoll epoll;                          // epoll
    MessageQueue msg_que;                 // 消息队列
    unordered_map<string, int> name_sock; // 在线用户名字、sock

    void handle_recv(); // 处理新连接和接收消息
    void handle_send(); // 处理发送消息
  public:
    Server(int port);
    Server(const char *port);
    int run();
};

Server::Server(const char *port) : epoll(EPOLL_SIZE) {
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(atoi(port));
}

int Server::run() {

    if ((listen_sock = socket(AF_INET, SOCK_STREAM, 0)) == 0) {
        cout << "socket() error" << endl;
        return -1;
    }
    if (bind(listen_sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
        cout << "bind() error" << endl;
        return -1;
    }
    if (listen(listen_sock, 5) < 0) {
        cout << "listen() error" << endl;
        return -1;
    }

    // cout << "listen() seuccess" << endl;
    thread connection_thread = thread([this]() { this->handle_recv(); });
    thread message_thread = thread([this]() { this->handle_send(); });

    connection_thread.join();
    message_thread.join();
    return 0;
}

void Server::handle_recv() {
    epoll.add_fd(listen_sock, EPOLLIN);
    int data_sock;

    while (1) {

        int event_cnt = epoll.wait(-1);
        if (event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }
        for (int i = 0; i < event_cnt; i++) {
            int cur_fd = epoll.get_event(i).data.fd;
            struct sockaddr_in clnt_addr;
            socklen_t clnt_addr_size;
            if (cur_fd == listen_sock) { // 新连接到达
                clnt_addr_size = sizeof(clnt_addr);
                data_sock = accept(listen_sock, (struct sockaddr *)&clnt_addr, &clnt_addr_size);
                epoll.add_fd(data_sock, EPOLLIN);
                printf("客户端连接: %d\n", data_sock);
            } else {
                char buf[BUF_SIZE];
                int str_len = read(cur_fd, buf, BUF_SIZE);
                // cout << "len " << str_len << endl;
                cout << buf << endl;
                if (str_len == 0) { // 连接关闭
                    epoll.delete_fd(cur_fd);
                    close(cur_fd);
                    printf("客户端关闭: %d\n", cur_fd);
                } else { // 处理接收消息逻辑
                    Message msg;
                    msg.deserialize((string)buf);
                    // cout << msg.type;
                    if (msg.type == 2) { // 登录消息
                        name_sock.insert({msg.from, cur_fd});
                        // cout << msg.from << "登录" << endl;
                    } else if (msg.type == 3) { // 登出消息
                        // cout << "1111111111?" << name_sock.size() << endl;
                        name_sock.erase(msg.from);
                        // cout << "1111111111?" << name_sock.size() << endl;
                    } else {
                        msg_que.push(msg);
                    }
                }
            }
        }
    }
}

void Server::handle_send() {
    while (1) {
        while (!msg_que.empty()) {
            Message msg = msg_que.pop();
            cout << msg.serialize().c_str() << endl;
            char buf[BUF_SIZE];
            snprintf(buf, BUF_SIZE, "%s", msg.serialize().c_str());
            if (msg.type == 0) { // 单发
                write(name_sock[msg.to], buf, strlen(buf));
            } else if (msg.type == 1) { // 群发
                for (const auto &pair : name_sock) {
                    write(pair.second, buf, strlen(buf));
                }
            } else {
                cout << "消息类型错误" << endl;
            }
        }
    }
}

int main(int argc, char *argv[]) {

    if (argc != 2){
        printf("Usage : %s <port>\n", argv[0]);
        exit(1);
    }
    Server server(argv[1]);
    server.run();
}