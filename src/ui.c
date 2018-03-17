#include <ncurses.h>
#include <unistd.h>
#include "ui.h"
#include "video_decoder.h"
#include "image_scaler.h"

static int ui_initialize()
{
  initscr();
  raw();
  keypad(stdscr, TRUE);
  noecho();
}

static int ui_draw_frame(const unsigned char* image, int width, int height)
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      mvaddch(i + 1, j + 1, (int) image[i * width + j]);
    }
  }
  refresh();
}

static void ui_cleanup()
{
  endwin();
}

int ui_start(const char* video_filename, int width, int height, ascii_options ascii_opts) {
  ui_initialize();
  frame_options opts;
  unsigned char* buf;
  int buf_size;

  video_initialize(video_filename);
  while (video_next_frame(&buf, &opts) > 0) {
    get_width_and_height(&width, &height, opts.width, opts.height);
    buf_size = width * height;
    unsigned char arr[buf_size];
    image_naive_scale(buf, opts.width, opts.height, arr, width, height);
    to_ascii(arr, buf_size, ascii_opts);
    ui_draw_frame(arr, width, height);
    usleep(20000);
  }

  video_cleanup();
  ui_cleanup();
}
