#include <stdio.h>
#include <stdlib.h>
#include <turbojpeg.h>
#include <math.h>
#include "bta.h"

typedef struct limits {
  unsigned char min;
  unsigned char max;
} limits;

int min_and_max(unsigned char* img, int img_size, limits* out) {
  unsigned char min = 0xFF, max = 0;
  for (int i = 0; i < img_size; i++) {
    unsigned char curr = img[i];
    if (curr < min) {
      min = curr;
    }
    if (curr > max) {
      max = curr;
    }
  }

  out->min = min;
  out->max = max;

  return 0;
}

int to_ascii(unsigned char* img, int img_size, ascii_options options) {
  limits l;
  min_and_max(img, img_size, &l);

  for (int i = 0; i < img_size; i++) {
    unsigned char curr = img[i];
    int bin = floor(options.char_set_size * (curr - l.min)/(l.max - l.min));
    if (bin >= options.char_set_size) {
      bin = options.char_set_size - 1;
    }
    img[i] = options.char_set[bin];
  }

  return 0;
}

ascii_image* jpeg_to_ascii(unsigned char* jpeg, int jpeg_size, ascii_image* ai, ascii_options options) {
  int width, height, jpegSubsamp, jpegColorspace;
  int pixel_format = TJPF_GRAY;
  int flags = TJFLAG_FASTDCT;
  int res;
  tjhandle t = tjInitDecompress();

  res = tjDecompressHeader3(t, jpeg, jpeg_size, &width, &height, &jpegSubsamp, &jpegColorspace);
  if (res == -1) {
    printf("Failed to decompress JPEG headers\n");
    return ai;
  }

  int pitch = width * tjPixelSize[TJPF_GRAY];
  int buf_size = pitch * height;
  unsigned char* buf = malloc(buf_size);
  res = tjDecompress2(t, jpeg, jpeg_size, buf, width, pitch, height, pixel_format, flags);
  if (res == -1) {
    printf("Failed to decompress JPEG\n");
    return ai;
  }

  tjDestroy(t);

  to_ascii(buf, buf_size, options);

  ai->image = buf;
  ai->size = buf_size;
  ai->height = height;
  ai->width = width;
  return ai;
}