#include <errno.h>
#include <error.h>
#include <libavcodec/codec.h>
#include <libavcodec/packet.h>
#include <libavutil/avutil.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>

#include <libavcodec/avcodec.h>
#include <libavfilter/avfilter.h>
#include <libavfilter/buffersink.h>
#include <libavfilter/buffersrc.h>
#include <libavformat/avformat.h>
#include <libavutil/buffer.h>
#include <libavutil/opt.h>
#include <libavutil/pixdesc.h>

int main(int argc, char **argv) {

	if (argc < 2) {
		perror("incorrect useage");
		return EXIT_FAILURE;
	}

	AVCodec *codec;
	AVBufferRef *device_ref;
	AVBufferRef *frames_ref;
	AVCodecContext *context;
	AVFormatContext *format_context;

	// codec = avcodec_find_decoder();
	format_context = avformat_alloc_context();

	if (avformat_open_input(&format_context, argv[1], NULL, NULL) < 0) {
		perror("Input could not be opened.");
		return EXIT_FAILURE;
	}

	// The file may contain more than on stream. Each stream can be a
	// video stream, an audio stream, a subtitle stream or something else.
	// also see 'enum CodecType' defined in avcodec.h
	// Find the first audio stream:
	int audioStreamID = -1;
	for (int i = 0; i < format_context->nb_streams; i++) {
		if (format_context->audio_codec->type == AVMEDIA_TYPE_AUDIO) {
			audioStreamID = i;
			break;
		}
	}

	// Get a pointer to the codec context for the audio stream
	/* context = format_context->; */

	// get the timebase for this stream. The presentation timestamp,
	// decoding timestamp and packet duration is expressed in timestamp
	// as unit:
	// e.g. if timebase is 1/90000, a packet with duration 4500
	// is 4500 * 1/90000 seconds long, that is 0.05 seconds == 20 ms.
	AVRational time_base = format_context->streams[audioStreamID]->time_base;

	// Find the decoder for the audio stream
	codec = avcodec_find_decoder(format_context->audio_codec->type);
	if (codec == NULL) {
		perror("Dedoder could not be found");
		return EXIT_FAILURE;
	}

	// Open the codec found suitable for this stream in the last step
	if (avcodec_open2(context, codec, NULL) < 0) {
		perror("Could not open codec");
		return EXIT_FAILURE;
	}

	// struct AVPacket: defined in avformat.h:
	// most important attributes in the strcture:
	//
	// packet.data     :the packets payload
	// packet.length   :length of payload in bytes
	// packet.pts      :presentation timestamp in untis of time_base. see above.
	// packet.duration :packet length in time_base unit
	// packet.dts      :decoding timestamp

	static AVPacket packet;
	int end_of_stream = 0;
	do {
		// read and forget packets until a packet with the
		// right stream ID (audioStreamID defined above) is found.
		do {
			// Read new frame
			if (av_read_frame(format_context, &packet) < 0) {
				end_of_stream = 1;
				break;
			}
		} while (packet.stream_index != audioStreamID);
		// here, a new audio packet from the stream is available

		/* cerr << "packetsize = " << packet.size << endl; */

		/* // to simulate a network connection, pass just the packet itself, */
		/* // the codecID and the presentation time to the receiver. This is */
		/* // the same information like a RTP connection can transport. */
		/* sendPacket(pCodecCtx->codec_id, (uint8_t *)packet.data, packet.size, */
		/* 		   packet.pts, time_base); */

	} while (!end_of_stream);

	// avcodec_get_hw_frames_parameters(context, device_ref, enum AVPixelFormat
	// hw_pix_fmt, frames_ref);

	avformat_close_input(&format_context);
	avformat_free_context(format_context);

	return EXIT_SUCCESS;
}
