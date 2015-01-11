GSTREAMER_FLAGS = `pkg-config --cflags gstreamer-0.10 gstreamer-app-0.10`
GSTREAMER_LD_LIBS = `pkg-config --libs gstreamer-0.10 gstreamer-app-0.10`

all: mediaplayer.o test

mediaplayer.o: mediaplayer.c mediaplayer.h
	$(CC) $(CFLAGS) $(GSTREAMER_FLAGS) -c mediaplayer.c $(GSTREAMER_LD_LIBS)

test: test.c mediaplayer.o
	$(CC) $(CFLAGS) $(GSTREAMER_FLAGS) -o test test.c mediaplayer.o $(GSTREAMER_LD_LIBS)

clean:
	@rm -fv mediaplayer.o
	@rm -fv test