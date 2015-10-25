#include <stdint.h>
#define MAX_LISTEN_BACKLOG 4096
#define BUFFER_SIZE 4096

struct epoll_event_handler {
    int fd;
    void (*handle)(struct epoll_event_handler*, uint32_t);
    void* closure;
};

struct backend_socket_event_data {
    struct epoll_event_handler* client_handler;
};

struct client_socket_event_data {
    struct epoll_event_handler* backend_handler;
};

struct server_socket_event_data {
    int epoll_fd;
    char* backend_addr;
    char* backend_port_str;
};

// netutils.c
void make_socket_non_blocking(int socket_fd);

// backend_socket.c
struct epoll_event_handler* create_backend_socket_handler(int backend_socket_fd,
                                                          struct epoll_event_handler* client_handler);
void handle_backend_socket_event(struct epoll_event_handler* self, uint32_t events);
void close_backend_socket(struct epoll_event_handler* self);
void close_backend_socket(struct epoll_event_handler* self);

// client_socket.c
struct epoll_event_handler* create_client_socket_handler(int client_socket_fd,
                                                         int epoll_fd,
                                                         char* backend_addr,
                                                         char* backend_port_str);
void handle_client_socket_event(struct epoll_event_handler* self, uint32_t events);
struct epoll_event_handler* connect_to_backend(struct epoll_event_handler* client_event_handler,
                                               int epoll_fd,
                                               char* backend_addr,
                                               char* backend_port_str);
void close_client_socket(struct epoll_event_handler* self);

// server_socket.c
int create_and_bind (char *port);
struct epoll_event_handler* create_server_socket_handler(int epoll_fd,
                                                         char* server_port_str,
                                                         char* backend_addr,
                                                         char* backend_port_str);
void handle_server_socket_event(struct epoll_event_handler* self, uint32_t events);
void handle_client_connection(int epoll_fd,
                              int client_socket_fd,
                              char* backend_addr,
                              char* backend_port_str);

// epoll_interface.c
void add_epoll_handler(int epoll_fd, struct epoll_event_handler* handler, uint32_t event_mask);
void do_reactor_loop(int epoll_fd);
