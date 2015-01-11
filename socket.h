#include<sys/socket.h>
#include<stdlib.h>
#include<stdio.h>

int Socket(int domain, int type, int protocol);

int Listen(int socket,int backlog);

int Connect(int socket,const struct sockaddr *address, socklen_t addr_len);

int Accept(int socket, struct sockaddr *addr,socklen_t len);

int Bind(int socket,const struct sockaddr* addr,socklen_t len);
