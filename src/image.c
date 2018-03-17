#include <stdio.h>
#include <stdlib.h>
#include <turbojpeg.h>
#include "image.h"
#include "image_scaler.h"

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

static int dump_image(const unsigned char* image, int width, int height)
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      char c = image[i * width + j];
      printf("%c", c);
    }
    printf("\n");
  }
  fflush(stdout);
  return 0;
}

static int decompress_jpeg(unsigned char* jpeg, int jpeg_size, image_options options, jpeg_image* ai) {
  int width, height, jpegSubsamp, jpegColorspace, res, scaling_factor_amount, scaled_height, scaled_width;
  int pixel_format = TJPF_GRAY;
  int flags = TJFLAG_FASTDCT;
  tjhandle t = tjInitDecompress();

  res = tjDecompressHeader3(t, jpeg, jpeg_size, &width, &height, &jpegSubsamp, &jpegColorspace);
  if (res == -1) {
    printf("%s\n", tjGetErrorStr());
    return -1;
  }

  if (options.preferred_width > 0 && options.preferred_width > 0) {
  
    tjscalingfactor* scaling_factor = tjGetScalingFactors(&scaling_factor_amount);
    for (int i = 0; i < scaling_factor_amount; i++) {
      scaled_width = TJSCALED(width, scaling_factor[i]);
      scaled_height = TJSCALED(height, scaling_factor[i]);
      if (scaled_width <= options.preferred_width && scaled_height <= options.preferred_height) break;
    }

    if (scaled_width > options.preferred_width || scaled_height > options.preferred_height) {
      scaled_width = width;
      scaled_height = height;
    }
  } else {
    scaled_width = width;
    scaled_height = height;
  }

  int pitch = scaled_width * tjPixelSize[TJPF_GRAY];
  int buf_size = pitch * scaled_height;
  unsigned char* buf = malloc(buf_size);

  res = tjDecompress2(t, jpeg, jpeg_size, buf, scaled_width, pitch, scaled_height, pixel_format, flags);
  if (res == -1) {
    printf("%s\n", tjGetErrorStr());
    return -1;
  }

  tjDestroy(t);

  ai->image = buf;
  ai->size = buf_size;
  ai->actual_height = scaled_height;
  ai->actual_width = scaled_width;
  return 0;
}

void show_image(unsigned char* image, int image_size, int width, int height, ascii_options ascii_opts) {
  jpeg_image jpeg;
  image_options img_opts = { 0, 0 };
  
  decompress_jpeg(image, image_size, img_opts, &jpeg);

  get_width_and_height(&width, &height, jpeg.actual_width, jpeg.actual_height);

  unsigned char output[width * height];
  image_naive_scale(jpeg.image, jpeg.actual_width, jpeg.actual_height, output, width, height);

  to_ascii(output, width * height, ascii_opts);

  // printf("%d:%d\n", jpeg.actual_width, jpeg.actual_height);

  dump_image(output, width, height);

  free(jpeg.image);
}
