
typedef struct ascii_image {
  unsigned char* image;
  int size;
  int width;
  int height;
} ascii_image;

typedef struct ascii_options {
  char* char_set;
  int char_set_size;
} ascii_options;

ascii_image* jpeg_to_ascii(unsigned char* jpeg, int jpeg_size, ascii_image* img, ascii_options options);