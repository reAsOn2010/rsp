#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include "rsp.h"

void make_socket_non_blocking(int socket_fd) {
    int flags;

    flags = fcntl(socket_fd, F_GETFL, 0);
    if (flags == -1) {
        perror("Couldn't get socket flags");
        exit(1);
    }

    flags |= O_NONBLOCK;
    if (fcntl(socket_fd, F_SETFL, flags) == -1) {
        perror("Couldn't set socket flags");
        exit(1);
    }
}
