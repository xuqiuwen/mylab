#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>

#include "send_icmp.h"

unsigned char sendbuff[BUFFSIZE] = {'\0'}; // 发送缓冲
unsigned char recvbuff[BUFFSIZE] = {'\0'}; // 接收缓冲

tunnel *tnel;                 // icmp 数据字段，分为三部分
int sock;                     // ICMP原始套接字
struct sockaddr_in dest_addr; // peer IP，sendto()要用
unsigned short total_len;     // ？不知道干嘛的
char *xorkey = "this is a key";

int main() {                                        // 主函数
    sock = socket(PF_INET, SOCK_RAW, IPPROTO_ICMP); // 分配一个ICMP原始套接字
    if (sock == -1) {                               // 分配失败，错误处理
        perror("error");
        exit(1);
    }

    char myname[NAMESIZE]; // 此进程用户名
    printf("Enter your name: ");
    scanf("%s", myname);

    char whoname[NAMESIZE]; // peer 进程用户名
    printf("Who you want to communicate: ");
    scanf("%s", whoname);

    char dst_ip[16]; // peer IP，字符串类型
    printf("Enter the dest IP: ");
    scanf("%s", dst_ip);

    init_icmp_packet(myname, whoname, dst_ip); // 初始化 icmp 包(首部和数据段头部)，设置全局变量，peer 的地址

    pid_t pid = fork(); // 创建子进程，将输入和输出分离
    if (pid == 0) {     // 子进程，负责接收数据
        while (1) {
            int n = recvfrom(sock, recvbuff, BUFFSIZE, 0, NULL, NULL);                           // 接收数据到 recvbuff 缓冲区，是一个 ip 包，而 sendto 发送缓冲区是 icmp包，这里是不一致的
            struct iphdr *iph = (struct iphdr *)(recvbuff);                                      // ip首部
            struct icmphdr *icmph = (struct icmphdr *)(recvbuff + sizeof(struct iphdr));         // icmp首部
            tunnel *recv = (tunnel *)(recvbuff + sizeof(struct iphdr) + sizeof(struct icmphdr)); // 数据字段

            decrypt();                          // 解密
            if (filter(icmph)) {                // 过滤
                printf("\t%s : ", recv->sname); // 打印发送者用户名
                printf("%s\n", recv->data);     // 打印信息
            }
            memset(recvbuff, 0, BUFFSIZE); // 清空接收缓冲区
        }
    }
    while (1) {                                               // 父进程，负责发送数据
        struct icmphdr *icmph = (struct icmphdr *)(sendbuff); // icmp 首部
        char *databuff = tnel->data;                          // databuff 指向 icmp 信息字段
        fgets(databuff, sizeof(tnel->data), stdin);           // 接收 icmp 数据的一部分信息到 databuff
        databuff[strcspn(databuff, "\n")] = '\0';             // \n 修改为 \0
        if (strlen(databuff) == 0)                            // 没有输入则不发送
            continue;
        senddata(databuff); // 发送数据
    }

    close(sock); // 关闭 icmp 套接字
    return 0;
}