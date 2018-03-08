#include <stdio.h>
#include <stdlib.h>
#include <turbojpeg.h>
#include <math.h>
#include "bta.h"

typedef struct limits {
  unsigned char min;
  unsigned char max;
} limits;

int min_and_max(const unsigned char* img, int img_size, limits* out) {
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

int to_ascii2(const unsigned char* img, int img_size, ascii_options options, unsigned char* output) {
  limits l;
  min_and_max(img, img_size, &l);

  for (int i = 0; i < img_size; i++) {
    unsigned char curr = img[i];
    int bin = floor(options.char_set_size * (curr - l.min)/(l.max - l.min));
    if (bin >= options.char_set_size) {
      bin = options.char_set_size - 1;
    }
    output[i] = options.char_set[bin];
  }

  return 0;
}

int display_image(const unsigned char* image, int width, int height) {
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      char c = image[i * width + j];
      if (c != '\n') {
        printf("%c", c);
      } else {
        printf("c");
      }
    }
    printf("\n");
  }
  fflush(stdout);
  return 0;
}

int decompress_jpeg(unsigned char* jpeg, int jpeg_size, image_options options, jpeg_image* ai) {
  int width, height, jpegSubsamp, jpegColorspace, res, scaling_factor_amount, scaled_height, scaled_width;
  int pixel_format = TJPF_GRAY;
  int flags = TJFLAG_FASTDCT;
  tjhandle t = tjInitDecompress();

  res = tjDecompressHeader3(t, jpeg, jpeg_size, &width, &height, &jpegSubsamp, &jpegColorspace);
  if (res == -1) {
    printf("%s\n", tjGetErrorStr());
    return -1;
  }

  if (options.preferred_width > 0 && options.preferred_width > 0) {
  
    tjscalingfactor* scaling_factor = tjGetScalingFactors(&scaling_factor_amount);
    for (int i = 0; i < scaling_factor_amount; i++) {
      scaled_width = TJSCALED(width, scaling_factor[i]);
      scaled_height = TJSCALED(height, scaling_factor[i]);
      if (scaled_width <= options.preferred_width && scaled_height <= options.preferred_height) break;
    }

    if (scaled_width > options.preferred_width || scaled_height > options.preferred_height) {
      scaled_width = width;
      scaled_height = height;
    }
  } else {
    scaled_width = width;
    scaled_height = height;
  }

  int pitch = scaled_width * tjPixelSize[TJPF_GRAY];
  int buf_size = pitch * scaled_height;
  unsigned char* buf = malloc(buf_size);

  res = tjDecompress2(t, jpeg, jpeg_size, buf, scaled_width, pitch, scaled_height, pixel_format, flags);
  if (res == -1) {
    printf("%s\n", tjGetErrorStr());
    return -1;
  }

  tjDestroy(t);

  ai->image = buf;
  ai->size = buf_size;
  ai->actual_height = scaled_height;
  ai->actual_width = scaled_width;
  return 0;
}