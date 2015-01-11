#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include<sys/socket.h>
#include<sys/types.h>
#include<netinet/tcp.h>
#include<arpa/inet.h>
#include<stdlib.h>
#include<stdio.h>
#include<assert.h>
#include<netdb.h>

#include "music_on_cloud.h"
#include "socket.h"
/* #include "media/mediaplayer.h" */
void client_main(char* server_IP, int l_port, char* media_dir){
  int sockfd = Socket(AF_INET,SOCK_STREAM,0);

  printf("Successfully created Socket\n");
  struct sockaddr_in server_info;
  memset((void*)&server_info,0,sizeof(server_info));

  struct hostent* server = gethostbyname(server_IP);
  assert(server != NULL);

  server_info.sin_family=AF_INET;
  server_info.sin_port=htons(l_port);
  server_info.sin_addr=*((struct in_addr*)server->h_addr);
  int fd = Connect(sockfd,(struct sockaddr*)&server_info,sizeof(server_info));
  printf("Successfully connected to server %s\n",server_IP);
  int term_flag=1;
  
  while(term_flag){
    break;
  }
  

}

int main(int argc, char **argv){
  int opt;
  int server_listen_port;
  char *server_IP;
  char *media_dir;
  
  /* Necessary for the player!!*/
  /* stream_info_t info; */
  
  /* A very easy way to parse command line arguments */
  while ((opt = getopt(argc, argv, "hs:p:d:")) != -1) {
    switch (opt)
      {
      case 's':
	server_IP = strdup(optarg);
	break;
      case 'd':
	media_dir = strdup(optarg);
	/* A few checks will be nice here...*/
	/* Convert the given dir to absolute path */
	break;
      case 'p':
	server_listen_port = atoi(optarg);
	/* To check or not to check? */
	break;
      default:
	printf("Usage: mocclient -s server_ip -p server_port -d directory \n"
	       "Options:\n"
	       "   -s <string>         The server IP address\n"
	       "   -p <int>            The listening port of the server\n"
	       "   -d <string>         The directory of the media files\n"
	       "   -h                  prints this help\n");
	exit(EXIT_FAILURE);
      }
  }
  
  printf("MediaOnCloud Client:\n"
	 "--> Server IP: %s\n"
	 "--> Server listen port: %d\n"
	 "--> Media directory: %s\n",
	 server_IP,
	 server_listen_port,
	 media_dir);
  
  /* Initialize the Gstreamer */
  init_mediaplayer();
  
  /* Your code starts here */
  
  client_main(server_IP,server_listen_port,media_dir);
  /* Your code ends here */
  
  /*
   * EXAMPLE: If you like to play a *local* file the following
   * should be enough:
   *
   */
  
  /* Choose local file */
  //info.network_stream = 0;
  /* Set filename and path */
  //info.filename = "/home/surligas/test.mp3";
  /* Play it!!! */
  //start_mediaplayer(&info);
  
  free(server_IP);
  free(media_dir);
  return 0;
}



