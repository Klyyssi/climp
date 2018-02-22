#include <stdio.h>
#include <stdlib.h>
#include <turbojpeg.h>

unsigned char* jpeg_to_ascii(unsigned char* jpeg, int jpeg_size) {
  unsigned char* buf = malloc(sizeof(unsigned char) * jpeg_size);
  int width, height, jpegSubsamp, jpegColorspace;
  int pitch = width * tjPixelSize[TJPF_GRAY];
  int pixel_format = TJPF_GRAY;
  int flags = 0;
  int res;
  tjhandle t = tjInitDecompress();

  res = tjDecompressHeader3(t, jpeg, jpeg_size, &width, &height, &jpegSubsamp, &jpegColorspace);
  if (res == -1) {
    printf("Failed to decompress headers for JPEG\n");
    return malloc(0);
  }

  res = tjDecompress2(t, jpeg, jpeg_size, buf, width, pitch, height, pixel_format, flags);
  if (res == -1) {
    printf("Failed to decompress JPEG\n");
    return malloc(0);
  }

  return buf;
}