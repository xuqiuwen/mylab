#include "Message.h"
#include <condition_variable>
#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H
#include <mutex>
#include <optional>
#include <queue>
using namespace std;
class MessageQueue {
  private:
    queue<Message> message_queue;
    mutable mutex mtx;
    condition_variable cond;

  public:
    void push(Message msg);
    Message pop();
    bool empty() const;
    size_t size() const;
};
#endif