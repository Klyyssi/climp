#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "bta.h"

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
  int opt;

  while((opt = getopt(argc, argv, "w:h:")) != -1) {
    switch (opt) {
      case 'w': width = atoi(optarg); break;
      case 'h': height = atoi(optarg); break;
      default:
        printf("Usage: bta -w 150 -h 90\n");
        exit(EXIT_FAILURE);
    }
  }

  unsigned char buf[200000];
  int size = read_stdin_simple(buf, 200000);

  jpeg_image jpeg;
  image_options img_opts = { width, height };
  ascii_options ascii_opts = {"X ", 2 };
  decompress_jpeg(buf, size, img_opts, &jpeg);
  to_ascii(jpeg.image, jpeg.actual_height * jpeg.actual_width, ascii_opts);

  printf("%d:%d\n", jpeg.actual_width, jpeg.actual_height);

  for (int i = 0; i < jpeg.actual_height; i++) {
    for (int j = 0; j < jpeg.actual_width; j++) {
      char c = jpeg.image[i * jpeg.actual_width + j];
      if (c != '\n') {
        printf("%c", c);
      } else {
        printf("c");
      }
    }
    printf("\n");
  }

  free(jpeg.image);

  return 0;
}
