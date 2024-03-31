#include "Epoll.h"

Epoll::Epoll() : max_size(100) {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        throw runtime_error("epoll_create1() error");
    }

    epoll_events = new epoll_event[max_size];
}

Epoll::Epoll(int max_size) : max_size(max_size) {
    epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        throw runtime_error("epoll_create1() error");
    }

    epoll_events = new epoll_event[max_size];
}

Epoll::~Epoll() {
    close(epoll_fd);
    delete[] epoll_events;
}

int Epoll::add_fd(int fd, uint32_t events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, fd, &event) == -1) {
        return -1;
    }
    return 0;
}

int Epoll::modify_fd(int fd, uint32_t events) {
    struct epoll_event event;
    event.events = events;
    event.data.fd = fd;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_MOD, fd, &event) == -1) {
        return -1;
    }
    return 0;
}

int Epoll::delete_fd(int fd) {
    if (epoll_ctl(epoll_fd, EPOLL_CTL_DEL, fd, nullptr) == -1) {
        return -1;
    }
    return 0;
}

int Epoll::wait(int timeout) {
    int event_count = epoll_wait(epoll_fd, epoll_events, max_size, timeout);
    if (event_count == -1) {
        return -1;
    }
    return event_count;
}

epoll_event Epoll::get_event(int index) {
    return epoll_events[index];
}