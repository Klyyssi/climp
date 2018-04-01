#include <ncurses.h>
#include <unistd.h>
#include <sys/time.h>
#include "ui.h"
#include "video_decoder.h"
#include "image_scaler.h"

#define CTRL(x)	(x&037)
#define KEY_QUIT 'q'
#define KEY_FIT_TO_SCREEN 's'
#define RESIZE_WINDOW 410

static int fit_to_screen = 0;

static struct timeval tv;

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
  move(0,0);
  for (int i = 0; i < height; i++) {
    for (int j = 0; j < width; j++) {
      addch((int) image[i * width + j]);
    }
    move(i, 0);
  }
  
  refresh();
}

static void ui_cleanup()
{
  endwin();
}

unsigned long current_time_microseconds()
{
  gettimeofday(&tv, NULL);
  return 1000000 *  tv.tv_sec + tv.tv_usec;
}

void display_time(int seconds)
{
  move(0,0);
  printw("%d:%d:%d", seconds / (60 * 60) ,seconds / 60 % 60, seconds % 60);
  refresh();
}

int ui_start(const char* video_filename, int default_width, int default_height, ascii_options ascii_opts)
{
  ui_initialize();
  frame_options opts;
  video_options vid_opts;
  unsigned char* buf;
  int buf_size;
  int user_input;
  int diff_microseconds = 0;
  unsigned long prev_time_microseconds;
  int width = default_width;
  int height = default_height;
  limits brightness_limits = {0, 255};

  if (width == 0 && height == 0) {
    fit_to_screen = 1;
    getmaxyx(stdscr, height, width);
  }

  video_initialize(video_filename, &vid_opts);
  prev_time_microseconds = current_time_microseconds();
  while (video_next_frame(&buf, &opts) > 0)
  {
    user_input = getch();
    if (user_input == CTRL('c') || user_input == KEY_QUIT)
    {
      break;
    }
    else if (user_input == KEY_LEFT)
    {
      video_rewind();
    }
    else if (user_input == KEY_RIGHT)
    {
      video_fast_forward();
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
    image_naive_scale(buf, opts.width, opts.height, opts.stride, arr, width, height);
    to_ascii(arr, buf_size, &ascii_opts, &brightness_limits);
    ui_draw_frame(arr, width, height);
    display_time(opts.time_in_seconds);
    diff_microseconds = current_time_microseconds() - prev_time_microseconds;
    int delay = 1/vid_opts.avg_frame_rate * 1000000 - diff_microseconds;
    if (delay > 0) {
      usleep(delay);
    }
    prev_time_microseconds = current_time_microseconds();
  }

  video_cleanup();
  ui_cleanup();

  return 0;
}
