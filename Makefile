all:
	gcc -D_POSIX_SOURCE -std=c99 -O2 rsp.c backend_socket.c client_socket.c epoll_interface.c netutils.c server_socket.c -o rsp.out
