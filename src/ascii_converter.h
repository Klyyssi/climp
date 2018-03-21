#ifndef ASCII_CONVERTER_H
#define ASCII_CONVERTER_H

typedef struct ascii_options {
  char* char_set;
  int char_set_size;
} ascii_options;

typedef struct limits {
  unsigned char min;
  unsigned char max;
} limits;

int to_ascii(unsigned char* img, int img_size, const ascii_options* options, limits* l);

#endif