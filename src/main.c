#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "bta.h"
#include "video_decoder.h"
#include "image_scaler.h"

#define BUF_SIZE 1024

// Returns char array if everything was ok
// On error -1
unsigned char* read_stdin() {
  unsigned char buf[BUF_SIZE];
  unsigned char* bytes = malloc(sizeof(unsigned char) * BUF_SIZE);
  int total_size = 0;
  int count = 0;
  int n = 1;
  do {
    count = read(0, buf, sizeof(buf));

    if (count < 0) {
      return -1;
    }

    if (total_size + count <= BUF_SIZE * n) {
      memcpy(bytes + total_size, buf, count);
    } else {
      n++;
      bytes = realloc(bytes, sizeof(unsigned char) * BUF_SIZE * n);
      memcpy(bytes + total_size, buf, count);
    }

    total_size += count;
  } while (count == sizeof(buf));
  
  return realloc(bytes, sizeof(unsigned char) * total_size);
}

int read_stdin_simple(unsigned char* buf, int buf_size) {
  int count = 0;
  int total_size = 0;
  unsigned char buf_helper[BUF_SIZE];
  while ((count = read(0, buf_helper, BUF_SIZE)) > 0) {
    if (total_size + count <= buf_size) {
      memcpy(buf + total_size, buf_helper, count);
    } else {
      return -1;
    }
    total_size += count;
  }
  
  return total_size;
}

void get_width_and_height(int* width, int* height, int actual_width, int actual_height) {
  if (*width == 0 && *height == 0)
  {
    (*width) = actual_width;
    (*height) = actual_height;
  }
  else if (*height == 0)
  {
    (*height) = image_get_height_preserve_aspect_ratio(actual_width, actual_height, *width);
  }
  else if (*width == 0)
  {
    (*width) = image_get_width_preserve_aspect_ratio(actual_width, actual_height, *height);
  }
}

void play_video(const char* filename, int width, int height, ascii_options ascii_opts) {
  frame_options opts;
  unsigned char* buf;
  int buf_size;
    
  video_initialize(filename);
  while (video_next_frame(&buf, &opts) > 0) {
    get_width_and_height(&width, &height, opts.width, opts.height);
    buf_size = width * height;
    unsigned char arr[buf_size];
    image_naive_scale(buf, opts.width, opts.height, arr, width, height);
    to_ascii(arr, buf_size, ascii_opts);
    display_image(arr, width, height);
    usleep(20000);
  }

  video_cleanup();
}

void show_image(int width, int height, ascii_options ascii_opts) {
  unsigned char buf[2000000];
  int size = read_stdin_simple(buf, 2000000);

  jpeg_image jpeg;
  image_options img_opts = { 0, 0 };
  
  decompress_jpeg(buf, size, img_opts, &jpeg);

  get_width_and_height(&width, &height, jpeg.actual_width, jpeg.actual_height);

  unsigned char output[width * height];
  image_naive_scale(jpeg.image, jpeg.actual_width, jpeg.actual_height, output, width, height);

  to_ascii(output, width * height, ascii_opts);

  // printf("%d:%d\n", jpeg.actual_width, jpeg.actual_height);

  display_image(output, width, height);

  free(jpeg.image);
}

int main(int argc, char **argv) {
  //unsigned char* buf = read_stdin();
  //if (buf == -1) {
  //  return -1;
  //}
  //printf("%s", buf);
  //free(buf);
  
  //unsigned char buf[200000];
  //int i = read_stdin_simple(buf, 200000);
  //printf("%s\n", buf);
  //printf("%d\n", i);

  int width = 0;
  int height = 0;
  int charset_length = 12;
  char* charset = ".,abcdABCDZX";
  char charset_allocated = 0;
  int opt;

  while((opt = getopt(argc, argv, "w:h:c:")) != -1) {
    switch (opt) {
      case 'w': width = atoi(optarg); break;
      case 'h': height = atoi(optarg); break;
      case 'c':
        charset_length = strlen(optarg);
        charset = malloc(sizeof(char) * charset_length);
        charset_allocated = 1;
        memcpy(charset, optarg, charset_length);
        break;
      default:
        printf("Usage: bta -w 150 -h 90\n");
        exit(EXIT_FAILURE);
    }
  }

  ascii_options ascii_opts = {charset, charset_length};

  if (optind >= argc) {
    show_image(width, height, ascii_opts);
  } else {
    play_video(argv[optind], width, height, ascii_opts);
  }

  if (charset_allocated) {
    free(charset);
  }

  return 0;
}
