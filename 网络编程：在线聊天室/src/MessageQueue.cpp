#include "MessageQueue.h"
// 添加消息
void MessageQueue::push(Message msg) {
    lock_guard<mutex> lock(mtx);
    message_queue.push(move(msg));
    cond.notify_one();
}

// 获取消息
Message MessageQueue::pop() {
    unique_lock<mutex> lock(mtx);
    cond.wait(lock, [this] { return !message_queue.empty(); });
    Message msg = move(message_queue.front());
    message_queue.pop();
    return msg;
}

// 检查队列是否为空
bool MessageQueue::empty() const {
    lock_guard<mutex> lock(mtx);
    return message_queue.empty();
}

// 获取队列大小
size_t MessageQueue::size() const {
    lock_guard<mutex> lock(mtx);
    return message_queue.size();
}