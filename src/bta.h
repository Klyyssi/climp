
typedef struct ascii_image {
  unsigned char* image;
  int size;
  int width;
  int height;
} ascii_image;

ascii_image* jpeg_to_ascii(unsigned char* jpeg, int jpeg_size, ascii_image* img);