/*
 * mediaplayer.c
 *
 *  Created on: Nov 4, 2014
 *      Author: surligas
 */

#include "mediaplayer.h"
#include <error.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <string.h>





/**
 * This is the callback that is called when the Gstreamer player
 * requires some data for the playback. The data source is a
 * UDP stream through the network. The socket descriptor can
 * be found inside the stream_info_t parameter.
 *
 * @param info the struct with the necessary fields
 * @return TRUE on success, FALSE otherwise
 *
 * For more help take a look at the feed_data_local() function
 *
 * !!! YOU SHOULD CHANGE THE CODE OF THIS FUNCTION !!!
 */
static gboolean
feed_data_network(stream_info_t *info)
{
	GstBuffer 	*gst_buffer;
	guint 		len;
	GstFlowReturn 	ret;
	uint8_t 	*network_buffer;

	gst_buffer = gst_buffer_new();

	/*
	 * Now get all the available data from network and store them in a local buffer
	 */

	/*
	 * FIXME!!!!
	 * In case there is no more data available, end-of-stream should be
	 * indicated.
	 *
	 * Change the true condition to something appropriate.
	 */
	if (1) {
		/* we are EOS, send end-of-stream */
		g_signal_emit_by_name(info->appsrc, "end-of-stream", &ret);
		return FALSE;
	}

	/* Set the appropriate len.
	 * This should be the total bytes received from the network*/
	len = BUFFER_SIZE; /* FIXME!!!! */

	/*
	 * Assign the network data just received to a GStreamer buffer.
	 *
	 * NOTE: Take care of len!!!!
	 */
	GST_BUFFER_DATA (gst_buffer) = network_buffer;
	GST_BUFFER_SIZE (gst_buffer) = len;

	g_signal_emit_by_name(info->appsrc, "push-buffer", gst_buffer, &ret);
	gst_buffer_unref(gst_buffer);

	return TRUE;
}


/**
 * This is the callback that is called when the Gstreamer player
 * requires some data for the playback. The data source is a
 * *local* file.
 *
 * @param info the struct with the necessary fields
 *
 * !!! NO NEED TO EDIT !!!
 */
static gboolean
feed_data_local(stream_info_t *info)
{
	GstBuffer 	*gst_buffer;
	guint 		len;
	GstFlowReturn 	ret;
	uint8_t 	*buffer;
	size_t		readn;

	printf("Need data\n");
	buffer = (uint8_t *)malloc(BUFFER_SIZE * sizeof(uint8_t));
	if (!buffer) {
		/* Could not allocate buffer. Set end of stream and exit */
		g_signal_emit_by_name(info->appsrc, "end-of-stream", &ret);
		return FALSE;
	}

	gst_buffer = gst_buffer_new();

	/* Read the next data from the file */
	readn = fread(buffer, sizeof(uint8_t), BUFFER_SIZE, info->fp);


	if (readn < 1) {
		/* we are EOS, send end-of-stream */
		printf("End of stream\n");
		g_signal_emit_by_name(info->appsrc, "end-of-stream", &ret);
		return FALSE;
	}

	/* Set the appropriate len. */
	len = readn;

	/*
	 * Assign the read data to a GStreamer buffer.
	 * No additional memory should be allocated.
	 * From now on, the buffer allocated by malloc is a GStreamer buffer.
	 *
	 * NOTE: Take care of len!!!!
	 */
	GST_BUFFER_MALLOCDATA (gst_buffer) = buffer;
	GST_BUFFER_SIZE (gst_buffer) = len;
	GST_BUFFER_DATA(gst_buffer) = GST_BUFFER_MALLOCDATA(gst_buffer);

	g_signal_emit_by_name(info->appsrc, "push-buffer", gst_buffer, &ret);

	/* This actually frees the memory that we have allocated before */
	gst_buffer_unref(gst_buffer);

	return TRUE;
}

/*
 * This signal callback triggers when appsrc needs data. Here, we add an idle handler
 * to the mainloop to start pushing data into the appsrc
 *
 * !!! NO NEED TO EDIT !!!
 */
static void
start_feed(GstElement *source, guint size, stream_info_t *sinfo)
{
	if (sinfo->sourceid == 0) {
		//g_print("Start feeding\n");
		if(sinfo->network_stream){
			sinfo->sourceid = g_idle_add((GSourceFunc) feed_data_network,
			                             sinfo);
		}
		else{
			sinfo->sourceid = g_idle_add((GSourceFunc) feed_data_local,
			                             sinfo);
		}
	}
}

/*
 * In order not to overflow the player with data,
 * this function is called whenever the player has enough data.
 *
 * !!! NO NEED TO EDIT !!!
 */
static void
stop_feed(GstElement * pipeline, stream_info_t *sinfo)
{
	if (sinfo->sourceid != 0) {
		//g_print("stop feeding\n");
		g_source_remove(sinfo->sourceid);
		sinfo->sourceid = 0;
	}
}

/*
 * This callback is called when playbin2 has constructed a source object to read
 * from. Since we provided the appsrc:// uri to playbin2, this will be the
 * appsrc that we must handle. We set up a signals to push data into appsrc.
 *
 * !!! NO NEED TO EDIT !!!
 */
static void
found_source(GObject * object, GObject * orig, GParamSpec * pspec,
             stream_info_t *sinfo)
{
	/* get a handle to the appsrc */
	g_object_get(orig, pspec->name, &sinfo->appsrc, NULL);

	sinfo->sourceid = 0;
	/*
	 * Configure the appsrc, we will push a buffer to appsrc when it needs more
	 * data.
	 *
	 * Depending the type of the play, (network or local file) the appropriate
	 * feeder callback is used.
	 */
	if (sinfo->network_stream) {
		printf("Network stream media source\n");
	}
	else{
		/*
		 * In case of a local file play, first open the file
		 * and assign it to the file descriptor of the
		 * stream_info_t struct.
		 */
		sinfo->fp = fopen(sinfo->filename, "r");
		printf("Local file media source\n");
		if (sinfo->fp == NULL) {
			perror("Could not open file for local play");
			return;
		}

	}
	g_signal_connect(sinfo->appsrc, "need-data", G_CALLBACK (start_feed),
	                 sinfo);
	g_signal_connect(sinfo->appsrc, "enough-data", G_CALLBACK (stop_feed),
	                 sinfo);
}

/*
 * !!! NO NEED TO EDIT !!!
 */
static gboolean
bus_message(GstBus * bus, GstMessage * message, stream_info_t *sinfo)
{

	switch (GST_MESSAGE_TYPE(message))
		{
	case GST_MESSAGE_ERROR:
		g_error("received error");
		g_main_loop_quit(sinfo->loop);
		break;
	case GST_MESSAGE_EOS:
		g_main_loop_quit(sinfo->loop);
		break;
	default:
		break;
		}
	return TRUE;
}


/**
 * Initializes the appropriate libraries of the GStreamer.
 * Should be used only once at your program.
 */
void
init_mediaplayer()
{
	gst_init(NULL, NULL);
}

/**
 * This function starts a media player. Depending the
 * containts of the stream_info_t struct, the media player
 * can play an online UDP stream or a local file.
 *
 * @param sinfo the struct containing all the necessary information
 * for the player.
 */
void
start_mediaplayer(stream_info_t *sinfo)
{
	GstBus *bus;

	/* create a mainloop to get messages */
	sinfo->loop = g_main_loop_new(NULL, TRUE);

	sinfo->playbin = gst_element_factory_make("playbin2", NULL);
	g_assert(sinfo->playbin);

	bus = gst_pipeline_get_bus(GST_PIPELINE(sinfo->playbin));

	/* add watch for messages */
	gst_bus_add_watch(bus, (GstBusFunc) bus_message, sinfo);

	/* set to read from appsrc */
	g_object_set(sinfo->playbin, "uri", "appsrc://", NULL);

	/* get notification when the source is created so that we get a handle to it
	 * and can configure it */
	g_signal_connect(sinfo->playbin, "deep-notify::source",
	                 (GCallback ) found_source, sinfo);

	/* go to playing and wait in a mainloop. */
	gst_element_set_state(sinfo->playbin, GST_STATE_PLAYING);

	/* this mainloop is stopped when we receive an error or EOS */
	g_main_loop_run(sinfo->loop);

	GST_DEBUG("stopping");

	gst_element_set_state(sinfo->playbin, GST_STATE_NULL);

	if(!sinfo->network_stream){
		fclose(sinfo->fp);
	}

	gst_object_unref(bus);
	g_main_loop_unref(sinfo->loop);
}


/**
 * Converts a sycnhsafe integer to a normal one.
 * NOTE: You will not use it. Its just for internal purposes.
 * @param sync_safe the synch-safe integer
 * @return the normal integer from the synchsafe integer.
 */
static uint32_t
ID3_sync_safe_to_int( uint8_t* sync_safe )
{
    uint32_t byte0 = sync_safe[0];
    uint32_t byte1 = sync_safe[1];
    uint32_t byte2 = sync_safe[2];
    uint32_t byte3 = sync_safe[3];

    return byte0 << 21 | byte1 << 14 | byte2 << 7 | byte3;
}


/**
 * This function returns the bit rate of a .mp3 file
 * @param filename the absolute path of the .mp3 file
 * @return the bit rate in kbps (Kilo bits per second)
 *
 * NOTE: Kilo is 1000 for the bit representation. Do not
 * get confused with KB which Kilo stands for 1024.
 */
size_t
get_mp3_bit_rate(const char *filename)
{
	uint32_t 	frame_header;
	uint32_t	ID3_len;
	uint8_t		synch_safe_word[4];
	uint8_t		ID3tag_footer;
	uint8_t		versionID;
	uint8_t		layer;
	uint8_t		bit_rate_index;
	uint16_t	search_sync;

	const uint16_t	sync = 0b11111111111;

	char		ID3tag[3];

	FILE 		*fp = fopen(filename, "r");
	if(!fp){
		perror("could not open file");
		return 0;
	}

	/* Skip the ID3 header, if any */
	fread(&ID3tag[0], sizeof(char), 3, fp);

	if(strncmp(ID3tag, "ID3", 3) == 0 ){
		/* Skip ID3 version */
		fseek(fp, 2, SEEK_CUR);

		/* Check for footer */
		fread(&ID3tag_footer, sizeof(uint8_t), 1, fp);
		ID3tag_footer = (ID3tag_footer >> 4) & 0x1;
		fread(&synch_safe_word[0], sizeof(uint8_t), 4, fp);
		ID3_len = ID3_sync_safe_to_int(synch_safe_word);

		/* Skip the ID3 header and its footer if exists */
		if(ID3tag_footer){
			fseek(fp, ID3_len + 10, SEEK_CUR);
		}
		else{
			fseek(fp, ID3_len, SEEK_CUR);
		}
	}

	/* Now its the time to find the start of a valid frame */
	fread(&search_sync, sizeof(uint16_t), 1, fp);
	search_sync = ntohs(search_sync);

	while( ((search_sync & 0b1111111111100000) >> 5) != sync){
		fseek(fp, -1, SEEK_CUR);
		fread(&search_sync, sizeof(uint16_t), 1, fp);
		search_sync = ntohs(search_sync);
	}

	fseek(fp, -2, SEEK_CUR);

	fread(&frame_header, sizeof(uint32_t), 1, fp);
	frame_header = ntohl(frame_header);

	versionID = (frame_header >> 19) & 0b11;
	layer = (frame_header >> 17) & 0b11;
	bit_rate_index = (frame_header >> 12) & 0b1111;

	fclose(fp);

	if(versionID == MPEGv1){
		return _mp3_bit_rates[bit_rate_index][versionID ^ layer];
	}
	else{
		if(layer == LAYER1){
			return _mp3_bit_rates[bit_rate_index][3];
		}
		else{
			return _mp3_bit_rates[bit_rate_index][4];
		}
	}
}

/**
 * This function returns the bit rate of a .flac file
 * @param filename the absolute path of the .flac file
 * @return the bit rate in kbps (Kilo bits per second)
 *
 * NOTE: Kilo is 1000 for the bit representation. Do not
 * get confused with KB which Kilo stands for 1024.
 */
size_t
get_flac_bit_rate(const char *filename)
{
	uint32_t	sample_rate;
	uint32_t	header_field;
	uint16_t	bits_per_sample;
	uint64_t	total_samples = 0;
	uint64_t	duration;

	FILE 		*fp = fopen(filename, "r");
	if(!fp){
		perror("could not open file");
		return 0;
	}

	fseek(fp, 4 + 10 + 4, SEEK_CUR);
	fread(&header_field, sizeof(uint32_t), 1, fp);

	header_field = ntohl(header_field);
	sample_rate = header_field >> 12;
	bits_per_sample = ((header_field >> 4) & 0b11111) + 1;

	total_samples =  (((uint64_t) header_field) & 0b1111) << 32;
	fread(&header_field, sizeof(uint32_t), 1, fp);
	fclose(fp);

	header_field = ntohl(header_field);

	total_samples |= (uint64_t) header_field;

	duration = total_samples / sample_rate;

	return (total_samples * bits_per_sample) / (1000 *duration);
}
