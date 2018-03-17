#ifndef ASCII_CONVERTER_H
#define ASCII_CONVERTER_H

typedef struct ascii_options {
  char* char_set;
  int char_set_size;
} ascii_options;

int to_ascii(unsigned char* img, int img_size, ascii_options options);

#endif