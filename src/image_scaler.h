#ifndef IMAGE_SCALER_H
#define IMAGE_SCALER_H

int image_naive_scale(const unsigned char* src_image, int src_width, int src_height, int src_stride, unsigned char* dst_image, int dst_width, int dst_height);

void get_width_and_height(int* width, int* height, int actual_width, int actual_height);

#endif