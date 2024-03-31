#ifndef __SEND_ICMP_H
#define __SEND_ICMP_H

#define NAMESIZE 10   // ICMP报文数据字段的用户名长度
#define BUFFSIZE 2048 // ICMP报文数据字段的传输数据长度、也是缓冲区长度

typedef struct icmp_tunnel {       // ICMP数据字段
    unsigned char sname[NAMESIZE]; // 发送方用户名
    unsigned char dname[NAMESIZE]; // 接收方用户名
    unsigned char data[BUFFSIZE];  // 传输信息
} tunnel;

void init_icmp_packet(const char *myname, const char *whoname, const char *dst_ip); // ICMP报文初始化
int senddata(char *data);                                                           // 发送数据
int filter(struct icmphdr *icmph);                                                  // ？过滤
void encrypt();                                                                     // 加密
void decrypt();                                                                     // 解密
void xorcrypt(char *input, char *key, char *output, int length);                    // xor加密解密

#endif