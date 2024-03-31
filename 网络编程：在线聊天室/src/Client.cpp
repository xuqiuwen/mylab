#include "Message.h"
#include <arpa/inet.h>
#include <chrono>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <thread>
#include <unistd.h>
#define BUF_SIZE 1000
using namespace std;

string getTime() {
    // 获取当前时间点
    auto now = chrono::system_clock::now();
    // 转换为 time_t 对象
    time_t now_time_t = chrono::system_clock::to_time_t(now);
    // 转换为 tm 结构
    tm now_tm = *localtime(&now_time_t);
    // 创建字符串流
    stringstream ss;
    // 使用流来格式化日期和时间
    ss << put_time(&now_tm, "%Y-%m-%d %H:%M:%S");
    // 获取格式化的字符串
    string datetime_str = ss.str();
    return datetime_str;
}

void receiveMessage(int sock) {
    char buf[BUF_SIZE];
    int str_len;

    while (1) {
        str_len = read(sock, buf, BUF_SIZE - 1);
        if (str_len == -1 || str_len == 0) {
            break; // 读取失败或者连接关闭
        }
        buf[str_len] = 0;
        cout << &buf[2] << endl;
    }
}

void sendMessage(int sock, string &name) {

    char buf[BUF_SIZE];
    string input;
    Message msg;

    while (true) {
        getline(cin, input); // 使用 getline 读取整行输入

        if (input == "help") {
            cout << "------------------\n"
                 << "help\n"
                 << "login yourname\n"
                 << "logout\n"
                 << ">messagetoall\n"
                 << "@someone>message\n"
                 << "------------------\n";
        } else if (input.substr(0, 6) == "login ") {
            name = input.substr(6);
            msg = Message(2, getTime(), name, "", "");
            // printf("Login as %s", name.c_str());
        } else if (input == "logout") {
            msg = Message(3, getTime(), name, "", "");
            // snprintf(buf, BUF_SIZE, "Logout");
            exit(1);
        } else if (input[0] == '>') {
            string message = input.substr(1);
            msg = Message(1, getTime(), name, "", message);
            // snprintf(buf, BUF_SIZE, "Broadcast message: %s", message.c_str());
        } else if (input[0] == '@') {
            size_t pos = input.find('>');
            if (pos != string::npos) {
                string to = input.substr(1, pos - 1);
                string message = input.substr(pos + 1);
                msg = Message(0, getTime(), name, to, message);
                // snprintf(buf, BUF_SIZE, "Private message to %s: %s", name.c_str(), message.c_str());
            } else {
                cout << "非法命令" << endl;
                continue;
            }
        } else {
            cout << "非法命令" << endl;
            continue;
        }
        snprintf(buf, BUF_SIZE, "%s", msg.serialize().c_str());
        write(sock, buf, strlen(buf));
    }
}

int main(int argc, char *argv[]) {
    int sock;
    char message[BUF_SIZE];
    int str_len;
    struct sockaddr_in serv_adr;

    if (argc != 3) {
        printf("Usage : %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    sock = socket(PF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        cout << "socket() error" << endl;
        return -1;
    }

    memset(&serv_adr, 0, sizeof(serv_adr));
    serv_adr.sin_family = AF_INET;
    serv_adr.sin_addr.s_addr = inet_addr(argv[1]);
    serv_adr.sin_port = htons(atoi(argv[2]));

    if (connect(sock, (struct sockaddr *)&serv_adr, sizeof(serv_adr)) == -1) {
        cout << "connect() error" << endl;
        perror(" ");
        return -1;
    }

    cout << "------------------\n"
         << "help\n"
         << "login yourname\n"
         << "logout\n"
         << ">messagetoall\n"
         << "@someone>message\n"
         << "------------------\n";

    string name;
    thread recv_thread(receiveMessage, sock);
    thread send_thread(sendMessage, sock, ref(name));

    send_thread.join();

    Message msg(3, getTime(), name, "", "");
    close(sock);
    return 0;
}
