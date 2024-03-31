#ifndef EPOLL_H
#define EPOLL_H
#include <stdexcept>
#include <sys/epoll.h>
#include <unistd.h>
using namespace std;
class Epoll {
  private:
    int epoll_fd;                     // epoll的文件描述符
    int max_size;                     // 事件最大个数
    struct epoll_event *epoll_events; // epoll事件数组

  public:
    Epoll(); // 默认大小100
    Epoll(int max_size);
    ~Epoll();

    int add_fd(int fd, uint32_t events);    // 添加套接字
    int modify_fd(int fd, uint32_t events); // 修改套接字，这里用不上
    int delete_fd(int fd);                  // 删除套接字
    int wait(int timeout);                  // 等待事件
    epoll_event get_event(int index);       // 获取第i个事件
};

#endif