#include "common.h"
#include "vbe.h"

struct vbe_mode_info_block current_vbe_mode_info_block;

bool vbe_get_info_block(struct vbe_info_block *ib) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ax = 0x4f00;
  in.di = (u16)ib;
  memcpy(ib->VbeSignature, "VBE2", 4);
  biosint(0x10, &in, &out);

  if (out.ax != 0x004f) {
    return false;
  }

  return true;
}

void vbe_set_video_mode(u16 mode) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ax = 0x4f02;
  in.bx = mode;

  biosint(0x10, &in, &out);
}

bool vbe_get_mode_info(u16 mode) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.eax = 0x4f01;
  in.cx = mode;
  in.di = (u16)&current_vbe_mode_info_block;
  biosint(0x10, &in, &out);

  if (out.ax != 0x004f) {
    return false;
  }

  return true;
}