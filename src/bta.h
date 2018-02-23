
typedef struct image_options {
  int preferred_width;
  int preferred_height;
} image_options;

typedef struct jpeg_image {
  unsigned char* image;
  int size;
  int actual_width;
  int actual_height;
} jpeg_image;

typedef struct ascii_options {
  char* char_set;
  int char_set_size;
} ascii_options;

int decompress_jpeg(unsigned char* jpeg, int jpeg_size, image_options options, jpeg_image* image);

int to_ascii(unsigned char* img, int img_size, ascii_options options);
