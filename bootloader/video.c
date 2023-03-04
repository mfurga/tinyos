#include "common.h"

extern boot_params_t boot_params;

static void store_video_mode(void) {
  struct regs in, out;
  regsinit(&in);
  in.ah = 0x0f;
  biosint(0x10, &in, &out);

  boot_params.video.mode = out.al & 0x7f;
  boot_params.video.page = out.bh;
}

static void store_cursor_position(void) {
  struct regs in, out;
  regsinit(&in);
  in.ah = 0x03;
  biosint(0x10, &in, &out);

  boot_params.video.x = out.dl;
  boot_params.video.y = out.dh;
}

static void store_resolution(void) {
  volatile int cols_addr = 0x44a;
  volatile int lines_addr = 0x484;

  boot_params.video.cols = *(u8 *)cols_addr;
  boot_params.video.lines = *(u8 *)lines_addr + 1;
}

void store_video(void) {
  store_video_mode();
  store_cursor_position();
  store_resolution();
}

void set_video(void) {
  store_video();

  /* TODO: Set video mode & save and restore screen. */
}

