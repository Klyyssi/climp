
#include "image_scaler.h"

int image_naive_scale(const unsigned char* src_image, int src_width, int src_height, unsigned char* dst_image, int dst_width, int dst_height)
{
  double x_scale_factor = src_width / (double) dst_width;
  double y_scale_factor = src_height / (double) dst_height;
  int src_x;
  int src_y;
  int c;
  int i,j;

  for (i = 0; i < dst_height; i++) {
    for (j = 0; j < dst_width; j++) {

      src_x = j * x_scale_factor;
      src_y = i * y_scale_factor;

      c = src_image[src_y * src_width + src_x];

      dst_image[i * dst_width + j] = c;
    }
  }

  return 0;
}

static int image_get_width_preserve_aspect_ratio(int src_width, int src_height, int dst_height)
{
  return dst_height * (src_width/(double) src_height);
}

static int image_get_height_preserve_aspect_ratio(int src_width, int src_height, int dst_width)
{
  return dst_width / (src_width / (double) src_height);
}

void get_width_and_height(int* width, int* height, int actual_width, int actual_height) {
  if (*width == 0 && *height == 0)
  {
    (*width) = actual_width;
    (*height) = actual_height;
  }
  else if (*height == 0)
  {
    (*height) = image_get_height_preserve_aspect_ratio(actual_width, actual_height, *width);
  }
  else if (*width == 0)
  {
    (*width) = image_get_width_preserve_aspect_ratio(actual_width, actual_height, *height);
  }
}
