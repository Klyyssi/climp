
int image_naive_scale(const unsigned char* src_image, int src_width, int src_height, unsigned char* dst_image, int dst_width, int dst_height);

int image_get_width_preserve_aspect_ratio(int src_width, int src_height, int dst_height);

int image_get_height_preserve_aspect_ratio(int src_width, int src_height, int dst_width);
