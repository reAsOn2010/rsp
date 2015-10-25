#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/epoll.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#include "rsp.h"

/*
void handle_client_connection(int client_socket_fd, char *backend_host, char*backend_port_str) {
    struct addrinfo hints;
    struct addrinfo *addrs;
    struct addrinfo *addrs_iter;
    int getaddrinfo_error;

    int backend_socket_fd;

    char buffer[BUFFER_SIZE];
    char respbuf[BUFFER_SIZE];
    int bytes_read;

    // init hints
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    getaddrinfo_error = getaddrinfo(backend_host, backend_port_str, &hints, &addrs);
    if (getaddrinfo_error != 0) {
        fprintf(stderr, "Couldn't find backend: %s\n", gai_strerror(getaddrinfo_error));
        exit(1);
    }
    for (addrs_iter = addrs; addrs_iter != NULL; addrs_iter = addrs_iter->ai_next) {
        backend_socket_fd = socket(addrs_iter->ai_family,
                                   addrs_iter->ai_socktype,
                                   addrs_iter->ai_protocol);
        if (backend_socket_fd == -1) {
            continue;
        }
        if (connect(backend_socket_fd,
                    addrs_iter->ai_addr,
                    addrs_iter->ai_addrlen) != -1) {
            break;
        }
        close(backend_socket_fd);
    }
    if (addrs_iter == NULL) {
        fprintf(stderr, "Couldn't connect to backend");
        exit(1);
    }
    // free it!
    freeaddrinfo(addrs);

    int count = 0;
    while (1) {
        if (bytes_read = read(client_socket_fd, buffer, BUFFER_SIZE)) {
            fprintf(stderr, "c %d", bytes_read);
            write(backend_socket_fd, buffer, bytes_read);
        }
        if (bytes_read = read(backend_socket_fd, respbuf, BUFFER_SIZE)) {
            fprintf(stderr, "b %d", bytes_read);
            write(client_socket_fd, buffer, bytes_read);
        }
        if (bytes_read == 0) {
            count += 1;
        }
        if (count > 5) {
            break;
        }
    }
    close(client_socket_fd);
}
*/

int main(int argc, char *argv[]) {

    if (argc != 4) {
        fprintf(stderr,
                "Usage: %s   \n",
                argv[0]);
        exit(1);
    }

    char* server_port_str = argv[1];
    char* backend_addr = argv[2];
    char* backend_port_str = argv[3];

    int epoll_fd = epoll_create1(0);
    if (epoll_fd == -1) {
        fprintf(stderr, "Could not create epoll FD");
        exit(1);
    }

    struct epoll_event_handler* server_socket_event_handler;
    server_socket_event_handler = create_server_socket_handler(epoll_fd,
                                                               server_port_str,
                                                               backend_addr,
                                                               backend_port_str);

    add_epoll_handler(epoll_fd, server_socket_event_handler, EPOLLIN);
    printf("Started. Listen on port %s.\n", server_port_str);
    do_reactor_loop(epoll_fd);

    return 0;
}
