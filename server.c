/*
 * server.c
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
	int opt;
	int listen_port;
	char *watched_dir;

	/* A very easy way to parse command line arguments */
	while ((opt = getopt(argc, argv, "hd:l:")) != -1) {
		switch (opt)
			{
		case 'd':
			watched_dir = strdup(optarg);
			/* A few checks will be nice here...*/
			/* Convert the given dir to absolute path */
			break;
		case 'l':
			listen_port = atoi(optarg);
			/* To check or not to check? */
			break;
		default:
			printf("Usage: mocserver -d directory_to_use -l listening_port\n"
			       "Options:\n"
			       "   -d <string>         The absolute path of the directory with the media files\n"
			       "   -l <int>            The listening port of the server\n"
			       "   -h                  prints this help\n");
			exit(EXIT_FAILURE);
			}
	}

	printf("MediaOnCloud Server:\n"
	       "--> Watched directory: %s\n"
	       "--> Listen port: %d\n",
	       watched_dir, listen_port);

	/* Your code starts here */


	/* Your code ends here */

	free(watched_dir);
	return 0;
}
