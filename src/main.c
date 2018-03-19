#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include "image.h"
#include "ui.h"

#define BUF_SIZE 1024

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
    unsigned char buf[2000000];
    int size = read_stdin_simple(buf, 2000000);
    if (size < 0) {
      printf("Image size too big\n");
      exit(EXIT_FAILURE);
    }
    show_image(buf, size, width, height, ascii_opts);
  } else {
    ui_start(argv[optind], width, height, ascii_opts);
  }

  if (charset_allocated) {
    free(charset);
  }

  return 0;
}
