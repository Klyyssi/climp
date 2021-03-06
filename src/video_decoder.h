#ifndef VIDEO_DECODER_H
#define VIDEO_DECODER_H

typedef struct frame_options {
  int width;
  int height;
  int stride;
  int time_in_seconds;
} frame_options;

typedef struct video_options {
  float avg_frame_rate;
} video_options;

int video_next_frame(unsigned char** buf, frame_options* options);

int video_initialize(const char* filename, video_options* options);

void video_cleanup();

int video_fast_forward();

int video_rewind();

#endif