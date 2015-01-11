# Change it to your disired compiler
CC := gcc

# The flags that will be passed to the compiler
CFLAGS := -Wall -g
LDFLAGS := -lpthread

GSTREAMER_FLAGS = `pkg-config --cflags gstreamer-0.10 gstreamer-app-0.10`
GSTREAMER_LD_LIBS = `pkg-config --libs gstreamer-0.10 gstreamer-app-0.10`

export CC
export CFLAGS

export GSTREAMER_FLAGS
export GSTREAMER_LD_LIBS

all: mocserver mocclient

# This will compile and execute the test program
# in order to find out if the appropriate GStreamer libraries.
# If all are ok, a video player that will play a video stream
# should appear.
#
# NOTE: Internet access is required to play the video!!!
# So execute "make test" as wait for the magic (or erros :-P )
test:
	$(MAKE) -C media $@

mocserver: server.c music_on_cloud.h media/mediaplayer.c media/mediaplayer.h
	$(MAKE) -C media mediaplayer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(GSTREAMER_FLAGS) -o mocserver server.c media/mediaplayer.o $(GSTREAMER_LD_LIBS)

mocclient: client.c music_on_cloud.h media/mediaplayer.c media/mediaplayer.h
	$(MAKE) -C media mediaplayer.o
	$(CC) $(CFLAGS) $(LDFLAGS) $(GSTREAMER_FLAGS) -o mocclient client.c media/mediaplayer.o $(GSTREAMER_LD_LIBS)

clean:
	$(MAKE) -C media $@
	@rm -fv mocserver
	@rm -fv mocclient