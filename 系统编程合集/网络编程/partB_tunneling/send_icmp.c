#include <arpa/inet.h>
#include <errno.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/ip_icmp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "send_icmp.h"

extern unsigned char sendbuff[BUFFSIZE];
extern unsigned char recvbuff[BUFFSIZE];

extern tunnel *tnel;
extern int sock;
extern struct sockaddr_in dest_addr;
extern unsigned short total_len;
extern char *xorkey;

static unsigned short
icmp_checksum( // 校验和计算
    unsigned short *ptr, int nbytes) {
    long sum;
    unsigned short oddbyte;
    unsigned short answer;
    sum = 0;
    while (nbytes > 1) {
        sum += *ptr++;
        nbytes -= 2;
    }
    if (nbytes == 1) {
        oddbyte = 0;
        *((uint8_t *)&oddbyte) = *(uint8_t *)ptr;
        sum += oddbyte;
    }
    sum = (sum >> 16) + (sum & 0xffff);
    sum += (sum >> 16);
    answer = ~sum;
    return answer;
}

// 这个函数好像有问题，需要修改
void init_icmp_packet(const char *myname, const char *whoname, const char *dst_ip) { // ICMP报文初始化
    tnel = (tunnel *)malloc(sizeof(tunnel));                                         // 分配数据字段空间
    memcpy(tnel->sname, myname, strlen(myname));                                     // 将此户名复制到ICMP报文
    memcpy(tnel->dname, whoname, strlen(whoname));                                   // 将peer用户名复制到ICMP报文

    struct icmphdr *icmph = (struct icmphdr *)(sendbuff); // ICMP首部字段设置到sendbuff
    icmph->type = ICMP_ECHO;                              // 为什么没有设置type字段，这里采用的好像是 ICMP_ECHO绕过过滤规则，见filter
    icmph->code = 0;                                      // 对于 ICMP echo request 或 reply (即 ping 请求和响应)，code 字段始终为0。
    icmph->un.echo.id = htons(1);                         // sub-type设为1
    icmph->un.echo.sequence = htons(1);                   // id 字段用于唯一地标识发送 ICMP echo 请求的进程

    dest_addr.sin_family = AF_INET;                  // 设置 peer 地址族
    inet_pton(AF_INET, dst_ip, &dest_addr.sin_addr); // 设置 peer 的IP，port 无需设置，sendto 会忽略，sendto 会识别套接字的类型
}

int senddata(char *data) { // 发送数据，这里要发送 icmp。sendbuff 的 icmp 头部已经设置完了，数据字段在 tnel 指针,data 其实是指向 tnel->data

    memcpy(sendbuff + sizeof(struct icmphdr), tnel, NAMESIZE * 2 + strlen(data) + 1); // 拷贝数据到发送缓冲区

    int length = sizeof(struct icmphdr) + NAMESIZE * 2 + strlen(data);
    ((struct icmphdr *)sendbuff)->checksum = icmp_checksum((unsigned short *)sendbuff, length); // 计算校验和

    encrypt(); // 加密

    int n = sendto(sock, sendbuff, length, 0, (struct sockaddr *)&dest_addr, sizeof(dest_addr)); // 发送
    if (n == -1) {
        perror("error");
    }
    return 0;
}

int filter(struct icmphdr *icmph) { // 过滤规则
    tunnel *recv = (tunnel *)(recvbuff + sizeof(struct iphdr) + sizeof(struct icmphdr));
    if (icmph->type != ICMP_ECHO || (strcmp(recv->dname, tnel->sname) != 0)) // 允许 ICMP_ECHO 报文和目的用户名一致的
        return 0;
    return 1;
}

void encrypt() {
    int hl = sizeof(struct icmphdr);
    xorcrypt(sendbuff + hl, xorkey, sendbuff + hl, NAMESIZE * 2 + strlen(sendbuff + hl + NAMESIZE * 2));
}

void decrypt() {
    int hl = sizeof(struct iphdr) + sizeof(struct icmphdr);
    xorcrypt(recvbuff + hl, xorkey, recvbuff + hl, NAMESIZE * 2 + strlen(recvbuff + hl + NAMESIZE * 2));
}

void xorcrypt(char *input, char *key, char *output, int length) {
    for (int i = 0; i < length; i++) {
        output[i] = input[i] ^ key[i % strlen(key)];
    }
}