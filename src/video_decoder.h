#ifndef VIDEO_DECODER_H
#define VIDEO_DECODER_H

typedef struct frame_options {
  int width;
  int height;
} frame_options;

int video_next_frame(unsigned char** buf, frame_options* options);

int video_initialize(const char* filename);

void video_cleanup();

#endif