#include <stdio.h>
#include <stdlib.h>
#include <turbojpeg.h>
#include "bta.h"

ascii_image* jpeg_to_ascii(unsigned char* jpeg, int jpeg_size, ascii_image* ai) {
  int width, height, jpegSubsamp, jpegColorspace;
  int pixel_format = TJPF_GRAY;
  int flags = TJFLAG_FASTDCT;
  int res;
  tjhandle t = tjInitDecompress();

  res = tjDecompressHeader3(t, jpeg, jpeg_size, &width, &height, &jpegSubsamp, &jpegColorspace);
  if (res == -1) {
    printf("Failed to decompress JPEG headers\n");
    return ai;
  }

  int pitch = width * tjPixelSize[TJPF_GRAY];
  int buf_size = pitch * height;
  unsigned char* buf = malloc(buf_size);
  res = tjDecompress2(t, jpeg, jpeg_size, buf, width, pitch, height, pixel_format, flags);
  if (res == -1) {
    printf("Failed to decompress JPEG\n");
    return ai;
  }

  tjDestroy(t);

  ai->image = buf;
  ai->size = buf_size;
  ai->height = height;
  ai->width = width;
  return ai;
}