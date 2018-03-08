
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "bta.h"
#include "video_decoder.h"

#include <libavcodec/avcodec.h>
#include <libavcodec/version.h>
#include <libavformat/avformat.h>

#define INBUF_SIZE (640 * 352 * 4)

static void pgm_save(unsigned char *buf, int wrap, int xsize, int ysize,
                     char *filename)
{
    FILE *f;
    int i;
    f = fopen(filename,"w");
    printf("Printing...\n");
    fprintf(f, "P5\n%d %d\n%d\n", xsize, ysize, 255);
    for (i = 0; i < ysize; i++)
        fwrite(buf + i * wrap, 1, xsize, f);
    fclose(f);
}
static int decode_write_frame(const char *outfilename, AVCodecContext *avctx,
                              AVFrame *frame, int *frame_count, AVPacket *pkt, int last)
{
    int len, got_frame;
    char buf[1024];
    len = avcodec_decode_video2(avctx, frame, &got_frame, pkt);
    if (len < 0) {
        fprintf(stderr, "Error while decoding frame %d\n", *frame_count);
        return len;
    }
    if (got_frame) {
        printf("Saving %sframe %3d\n", last ? "last " : "", *frame_count);
        fflush(stdout);
        /* the picture is allocated by the decoder, no need to free it */
        snprintf(buf, sizeof(buf), outfilename, *frame_count);
        pgm_save(frame->data[0], frame->linesize[0],
                 frame->width, frame->height, buf);
        (*frame_count)++;
    }
    if (pkt->data) {
        pkt->size -= len;
        pkt->data += len;
    }
    return 0;
} 

static AVFormatContext* format = NULL;
static AVCodecContext* codecContext = NULL;
static AVCodec* codec = NULL;
static AVStream* stream = NULL;
static AVFrame* frame = NULL; // Contains decoded video frame
static AVPacket packet; // Contains encoded video frame(s)

static int cleanup() {
    av_free_packet(&packet);
    avcodec_close(codecContext);
    av_frame_free(&frame);
    avformat_close_input(&format);
}

int initialize_video(const char* filename)
{
    int ret = 0;
    int stream_index = 0;

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

    return 0;
}

int next_frame(unsigned char** buf, frame_options* options) {
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
                options->width = frame->linesize[0];
                (*buf) = (unsigned char*) frame->data[0];
                return ret;
            }
            packet.data += ret;
            packet.size -= ret;
        } while (packet.size > 0);
    }

    return 0;
}

static void video_decode_example(const char *outfilename, const char *filename)
{
    AVCodec *codec;
    AVCodecContext *c= NULL;
    int frame_count;
    FILE *f;
    AVFrame *frame;
    uint8_t inbuf[INBUF_SIZE + AV_INPUT_BUFFER_PADDING_SIZE];
    AVPacket avpkt;
    av_init_packet(&avpkt);
    /* set end of buffer to 0 (this ensures that no overreading happens for damaged mpeg streams) */
    memset(inbuf + INBUF_SIZE, 0, AV_INPUT_BUFFER_PADDING_SIZE);
    printf("Decode video file %s to %s\n", filename, outfilename);
    /* find the mpeg1 video decoder */
    codec = avcodec_find_decoder(AV_CODEC_ID_H264);
    if (!codec) {
        fprintf(stderr, "Codec not found\n");
        exit(1);
    }
    c = avcodec_alloc_context3(codec);
    if (!c) {
        fprintf(stderr, "Could not allocate video codec context\n");
        exit(1);
    }

    if (codec->capabilities & AV_CODEC_CAP_TRUNCATED)
        c->flags |= AV_CODEC_FLAG_TRUNCATED; // we do not send complete frames
    // Set video width and height
    c->height = 352;
    c->width = 640;
    // c->frame_size is for audio only
    /* For some codecs, such as msmpeg4 and mpeg4, width and height
       MUST be initialized there because this information is not
       available in the bitstream. */
    /* open it */
    if (avcodec_open2(c, codec, NULL) < 0) {
        fprintf(stderr, "Could not open codec\n");
        exit(1);
    }
    f = fopen(filename, "rb");
    if (!f) {
        fprintf(stderr, "Could not open %s\n", filename);
        exit(1);
    }
    frame = av_frame_alloc();
    if (!frame) {
        fprintf(stderr, "Could not allocate video frame\n");
        exit(1);
    }
    frame_count = 0;
    for (;;) {
        avpkt.size = fread(inbuf, 1, INBUF_SIZE, f);
        if (avpkt.size == 0)
            break;
        /* NOTE1: some codecs are stream based (mpegvideo, mpegaudio)
           and this is the only method to use them because you cannot
           know the compressed data size before analysing it.
           BUT some other codecs (msmpeg4, mpeg4) are inherently frame
           based, so you must call them with all the data for one
           frame exactly. You must also initialize 'width' and
           'height' before initializing them. */
        /* NOTE2: some codecs allow the raw parameters (frame size,
           sample rate) to be changed at any frame. We handle this, so
           you should also take care of it */
        /* here, we use a stream based decoder (mpeg1video), so we
           feed decoder and see if it could decode a frame */
        avpkt.data = inbuf;
        printf("Size: %d\n", avpkt.size);
        while (avpkt.size > 0)
            if (decode_write_frame(outfilename, c, frame, &frame_count, &avpkt, 0) < 0) {
                fprintf(stderr, "Exiting...\n");
                exit(1);
            }
    }
    /* some codecs, such as MPEG, transmit the I and P frame with a
       latency of one frame. You must do the following to have a
       chance to get the last frame of the video */
    avpkt.data = NULL;
    avpkt.size = 0;
    decode_write_frame(outfilename, c, frame, &frame_count, &avpkt, 1);
    fclose(f);
    avcodec_close(c);
    av_free(c);
    av_frame_free(&frame);
    printf("\n");
}

int main()
{
    frame_options opts;
    ascii_options ascii_opts = { ".abcABC", 7 };
    unsigned char* buf;
    int buf_size;
    
    initialize_video("vid4.mp4");
    while (next_frame(&buf, &opts) > 0) {
        buf_size = opts.width * opts.height;
        unsigned char arr[buf_size];
        to_ascii2(buf, buf_size, ascii_opts, arr);
        display_image(arr, opts.width, opts.height);
        usleep(20000);
    }

    cleanup();
    
    return 0;
}
