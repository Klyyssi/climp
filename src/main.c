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

int main() {
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

  unsigned char buf[200000];
  int size = read_stdin_simple(buf, 200000);
  for (int i = 0; i < size; i++) {
    //printf("%c", buf[i]);
  }

  ascii_image img;
  jpeg_to_ascii(buf, size, &img);
  for (int i = 0; i < img.height; i++) {
    for (int j = 0; j < img.width; j++) {
      printf("%c", img.image[i * img.width + j]);
    }
    printf("\n");
  }

  return 0;
}
