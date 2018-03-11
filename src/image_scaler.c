
#include "image_scaler.h"

int image_naive_scale(const unsigned char* src_image, int src_width, int src_height, unsigned char* dst_image, int dst_width, int dst_height)
{
  double x_scale_factor = src_width / (double) dst_width;
  double y_scale_factor = src_height / (double) dst_height;

  for (int i = 0; i < dst_height; i++) {
    for (int j = 0; j < dst_width; j++) {

      int src_x = j * x_scale_factor;
      int src_y = i * y_scale_factor;

      char c = src_image[src_y * src_width + src_x];

      dst_image[i * dst_width + j] = c;
    }
  }

  return 0;
}

int image_get_width_preserve_aspect_ratio(int src_width, int src_height, int dst_height)
{
  return dst_height * (src_width/(double) src_height);
}

int image_get_height_preserve_aspect_ratio(int src_width, int src_height, int dst_width)
{
  return dst_width / (src_width / (double) src_height);
}
