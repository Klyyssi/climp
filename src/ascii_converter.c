#include <math.h>
#include "ascii_converter.h"

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

int to_ascii(unsigned char* img, int img_size, const ascii_options* options) {
  limits l;
  min_and_max(img, img_size, &l);

  for (int i = 0; i < img_size; i++) {
    unsigned char curr = img[i];
    int bin = floor(options->char_set_size * (curr - l.min)/(l.max - l.min));
    if (bin >= options->char_set_size) {
      bin = options->char_set_size - 1;
    }
    img[i] = options->char_set[bin];
  }

  return 0;
}