#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>

#include "rsp.h"

void add_epoll_handler(int epoll_fd, struct epoll_event_handler* handler, uint32_t event_mask) {
    struct epoll_event event;

    event.data.ptr = handler;
    event.events = event_mask;
    if (epoll_ctl(epoll_fd, EPOLL_CTL_ADD, handler->fd, &event) == -1) {
        perror("Couldn't register server socket with epoll");
        exit(-1);
    }
}

void do_reactor_loop(int epoll_fd) {
    struct epoll_event current_epoll_event;

    while (1) {
        struct epoll_event_handler* handler;

        epoll_wait(epoll_fd, &current_epoll_event, 1, -1);
        handler = (struct epoll_event_handler*) current_epoll_event.data.ptr;
        handler->handle(handler, current_epoll_event.events);
    }
}
