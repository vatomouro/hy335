/*
 * client.c
 *
 *  Created on: Nov 5, 2014
 *      Author: surligas
 */

#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "music_on_cloud.h"

#include "media/mediaplayer.h"


int
main(int argc, char **argv)
{
	int 		opt;
	int 		server_listen_port;
	char 		*server_IP;
	char 		*media_dir;

	/* Necessary for the player!!*/
	stream_info_t 	info;

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



