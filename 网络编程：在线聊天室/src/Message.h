#ifndef MESSAGE_H
#define MESSAGE_H
#include <sstream>
#include <string>
#include <vector>
using namespace std;
class Message {
  public:
    int type;       // 消息类型 0单发 1群发 2登录 3登出
    string time;    // 时间
    string from;    // 发送者
    string to;      // 接收者
    string content; // 消息
    Message() {}
    Message(int type, string time, string from, string to, string content);
    string serialize() const;             // 序列化
    void deserialize(const string &data); // 反序列化
};
#endif