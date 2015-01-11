#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>

#define FAILURE(msg) perror(msg);abort();

int Socket(int domain, int type, int protocol){

  int sockfd = socket(domain,type,protocol);
  if(sockfd==-1) FAILURE("socket creation failure");

  int flag=1;
  if(setsockopt(sockfd,IPPROTO_TCP,TCP_NODELAY,&flag,sizeof(int))==-1) FAILURE("setsockopt")
  return sockfd;
}

int Listen(int socket,int backlog){
  int fd = listen(socket,backlog);
  if(fd==-1) FAILURE("listen call failure");
  return fd;
}

int Connect(int socket,const struct sockaddr *address, socklen_t addr_len){
  int fd = connect(socket,address,addr_len);
  if(fd==-1) FAILURE("connect failure");
  return fd;
}

int Accept(int socket, struct sockaddr *addr,socklen_t len){
  int fd = Accept(socket,addr,len);
  if(fd==-1) FAILURE("accept failure");
  return fd;
}

int Bind(int socket,const struct sockaddr* addr,socklen_t len){
  int fd = bind(socket,addr,len);
  if(fd==-1) FAILURE("bind failure")
  return fd;
}
