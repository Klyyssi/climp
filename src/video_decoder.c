
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "video_decoder.h"

#include <libavcodec/avcodec.h>
#include <libavcodec/version.h>
#include <libavformat/avformat.h>

static AVFormatContext* format = NULL;
static AVCodecContext* codecContext = NULL;
static AVCodec* codec = NULL;
static AVStream* stream = NULL;
static AVFrame* frame = NULL; // Contains decoded video frame
static AVPacket packet; // Contains encoded video frame(s)
static int stream_index = 0;

#define REWIND_FAST_FORWARD_SECONDS 10

void video_cleanup()
{
    av_free_packet(&packet);
    avcodec_close(codecContext);
    av_frame_free(&frame);
    avformat_close_input(&format);
}

int video_initialize(const char* filename, video_options* options)
{
    int ret = 0;

    av_register_all();

    // Suppress decoder messages ---------------------------------------
    av_log_set_level(AV_LOG_QUIET);
    // -----------------------------------------------------------------

    // Open file and allocate format context ---------------------------
    ret = avformat_open_input(&format, filename, NULL, NULL);
    if (ret < 0) {
        printf("Could not open input stream. Error: %d\n", ret);
        return -1;
    }
    // -----------------------------------------------------------------

    // Get stream information ------------------------------------------
    ret = avformat_find_stream_info(format, NULL);
    if (ret < 0) {
        printf("Could not read stream information\n");
        return -1;
    }
    // -----------------------------------------------------------------

    // Figure out AVCodecContext, and find the AVCodec related to it ---
    ret = av_find_best_stream(format, AVMEDIA_TYPE_VIDEO, -1, -1, NULL, 0);
    if (ret < 0) {
        printf("Could not find stream in input file\n");
        return -1;
    }
    stream_index = ret;
    stream = format->streams[stream_index];
    codecContext = stream->codec;
    codec = avcodec_find_decoder(codecContext->codec_id);
    if (!codec) {
        printf("Could not find codec_id: %d\n", codecContext->codec_id);
        return -1;
    }
    // -----------------------------------------------------------------

    // Try to open the codec -------------------------------------------
    ret = avcodec_open2(codecContext, codec, NULL);
    if (ret < 0) {
        printf("Could not open codec_id: %d\n", codecContext->codec_id);
        return -1;
    }
    // -----------------------------------------------------------------

    // Dump information ------------------------------------------------
    // av_dump_format(format, 0, filename, 0);
    // -----------------------------------------------------------------

    // Allocate frame --------------------------------------------------
    frame = av_frame_alloc();
    // -----------------------------------------------------------------

    // Initialize packet -----------------------------------------------
    av_init_packet(&packet);
    packet.data = NULL;
    packet.size = 0;
    // -----------------------------------------------------------------

    // Set video options -----------------------------------------------
    options->avg_frame_rate = (float) stream->avg_frame_rate.num / stream->avg_frame_rate.den;
    // -----------------------------------------------------------------

    return 0;
}

int video_fast_forward()
{
    int ret = 0;
    AVRational s = stream->time_base;
    ret = av_seek_frame(format, stream_index, frame->pkt_pts + AV_TIME_BASE * REWIND_FAST_FORWARD_SECONDS * s.num / s.den, 0);
    avcodec_flush_buffers(codecContext);
    if (ret < 0) {
        return -1;
    }
    return 0;
}

int video_rewind()
{
    int ret = 0;
    AVRational s = stream->time_base;
    ret = av_seek_frame(format, stream_index, frame->pkt_pts - AV_TIME_BASE * REWIND_FAST_FORWARD_SECONDS * s.num / s.den, AVSEEK_FLAG_BACKWARD);
    avcodec_flush_buffers(codecContext);
    if (ret < 0) {
        return -1;
    }
    return 0;
}

int video_next_frame(unsigned char** buf, frame_options* options)
{
    int ret = 0;
    int got_frame = 0;

    while (av_read_frame(format, &packet) >= 0) {
        do {
            ret = avcodec_decode_video2(codecContext, frame, &got_frame, &packet);
            if (ret < 0) {
                break;
            }
            if (got_frame) {
                options->height = frame->height;
                options->stride = frame->linesize[0];
                options->width = frame->width;
                options->time_in_seconds = frame->pkt_pts * stream->time_base.num / stream->time_base.den;
                (*buf) = (unsigned char*) frame->data[0];
                return ret;
            }
            packet.data += ret;
            packet.size -= ret;
        } while (packet.size > 0);
    }

    return 0;
}
