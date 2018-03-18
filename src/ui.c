#include <ncurses.h>
#include <unistd.h>
#include "ui.h"
#include "video_decoder.h"
#include "image_scaler.h"

#define CTRL(x)	(x&037)
#define KEY_QUIT 'q'
#define KEY_FIT_TO_SCREEN 's'
#define RESIZE_WINDOW 410

static int fit_to_screen = 0;

static int ui_initialize()
{
  initscr();
  raw();
  keypad(stdscr, TRUE);
  // Use non-blocking user inputs
  nodelay(stdscr, TRUE);
  noecho();
}

static int ui_draw_frame(const unsigned char* image, int width, int height)
{
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      mvaddch(i, j, (int) image[i * width + j]);
    }
  }
  refresh();
}

static void ui_cleanup()
{
  endwin();
}

int ui_start(const char* video_filename, int default_width, int default_height, ascii_options ascii_opts)
{
  ui_initialize();
  frame_options opts;
  unsigned char* buf;
  int buf_size;
  int user_input;
  int width = default_width;
  int height = default_height;

  video_initialize(video_filename);
  while (video_next_frame(&buf, &opts) > 0) {

    user_input = getch();
    if (user_input == CTRL('c') || user_input == KEY_QUIT)
    {
      break;
    }
    else if (user_input == KEY_FIT_TO_SCREEN)
    {
      fit_to_screen ^= 1;
      if (fit_to_screen)
      {
        getmaxyx(stdscr, height, width);
      }
      else
      {
        width = default_width;
        height = default_height;
        clear();
      }
    }
    else if (user_input == RESIZE_WINDOW)
    {
      if (fit_to_screen)
      {
        getmaxyx(stdscr, height, width);
      }
    }

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

  return 0;
}
