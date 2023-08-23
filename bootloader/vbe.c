#include "common.h"
#include "vbe.h"

#include "multiboot.h"

#define MAX_VIDEO_MODES 16

#define PREFERRED_RES_X 800
#define PREFERRED_RES_Y 600

#define MIN_RES_X 640
#define MIN_RES_Y 480
#define MIN_BPP 32

extern struct multiboot_info mbi;

struct VbeInfoBlock __ib;
struct VbeModeInfoBlock __mib;

static u16 selected_video_mode;

struct video_mode {
  u16 mode;
  u16 xres;
  u16 yres;
  u16 bpp;
  bool is_text_mode;
};

static struct video_mode video_modes[MAX_VIDEO_MODES];
static size_t video_modes_count = 0;

void vga_set_video_mode(u8 mode) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ah = 0;
  in.al = mode;

  biosint(0x10, &in, &out);
}

static void video_mode_append(struct video_mode vm) {
  if (video_modes_count >= ARRAY_SIZE(video_modes)) {
    return;
  }
  video_modes[video_modes_count++] = vm;
}

static inline bool is_ok_res(u16 x, u16 y) {
  return x >= MIN_RES_X && y >= MIN_RES_Y;
}

static inline bool is_ok_bpp(u16 bpp) {
  return bpp >= MIN_BPP;
}

static bool is_ok_video_mode(const struct VbeModeInfoBlock *mib) {
  if (!is_ok_res(mib->XResolution, mib->YResolution)) {
    return false;
  }

  if (!is_ok_bpp(mib->BitsPerPixel)) {
    return false;
  }

  return true;
}

bool vbe_get_info_block(struct VbeInfoBlock *ib) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.ax = 0x4f00;
  in.di = (u32)ib;
  memcpy(ib->VbeSignature, "VBE2", 4);
  biosint(0x10, &in, &out);

  if (out.ax != 0x004f) {
    return false;
  }

  return true;
}

bool vbe_get_mode_info_block(u16 mode, struct VbeModeInfoBlock *mib) {
  struct regs in, out;
  regsinit(&in);
  regsinit(&out);

  in.eax = 0x4f01;
  in.cx = mode | VBE_MODE_USE_LINEAR_FB;
  in.di = (u32)mib;
  biosint(0x10, &in, &out);

  if (out.ax != 0x004f) {
    return false;
  }

  return true;
}

static void multiboot_save_framebuffer_info(u16 mode) {
  if (!vbe_get_mode_info_block(mode, &__mib)) {
    return;
  }

  mbi.flags |= MULTIBOOT_INFO_FRAMEBUFFER_INFO;

  mbi.framebuffer_addr = (u64)__mib.PhysBasePtr;
  if (__ib.VbeVersion >= 0x300) {
    mbi.framebuffer_pitch = (u32)__mib.LinBytesPerScanLine;
  }
  mbi.framebuffer_width = (u32)__mib.XResolution;
  mbi.framebuffer_height = (u32)__mib.YResolution;
  mbi.framebuffer_bpp = (u8)__mib.BitsPerPixel;
  mbi.framebuffer_type = MULTIBOOT_FRAMEBUFFER_TYPE_RGB;

  mbi.framebuffer_red_field_position = __mib.RedFieldPosition;
  mbi.framebuffer_red_mask_size = __mib.RedMaskSize;
  mbi.framebuffer_green_field_position = __mib.GreenFieldPosition;
  mbi.framebuffer_green_mask_size = __mib.GreenMaskSize;
  mbi.framebuffer_blue_field_position = __mib.BlueFieldPosition;
  mbi.framebuffer_blue_mask_size = __mib.BlueMaskSize;
}

void vbe_set_video_mode(void) {
  struct regs in, out;

  regsinit(&in);
  regsinit(&out);
  u16 mode = selected_video_mode;

  in.ax = 0x4f02;
  in.bx = mode | VBE_MODE_USE_LINEAR_FB;

  biosint(0x10, &in, &out);

  if (out.ax != 0x004f) {
    panic("Faield to set VBE video mode to %u. Error code: %x\n",
      mode, out.ax);
  }

  multiboot_save_framebuffer_info(mode);
}

void print_video_mode_info(void) {
  size_t idx;
  for (idx = 0; idx < video_modes_count; idx++) {
    if (selected_video_mode == video_modes[idx].mode) {
      break;
    }
  }

  if (idx == video_modes_count) {
    return;
  }

  if (video_modes[idx].is_text_mode) {
    printf("%u x %u text mode\n",
      video_modes[idx].xres,
      video_modes[idx].yres
    );
  } else {
    printf("%u x %u x %u\n",
      video_modes[idx].xres,
      video_modes[idx].yres,
      video_modes[idx].bpp
    );
  }
}

void print_video_modes(void) {
  const char *prefix;
  printf("Supported video modes:\n");

  for (size_t i = 0; i < video_modes_count; i++) {
    prefix = "";

    if (video_modes[i].mode == selected_video_mode) {
      prefix = C_WHILE;
    }

    if (video_modes[i].is_text_mode) {
      printf("%s [%u] %u x %u text mode\n",
        prefix,
        i,
        video_modes[i].xres,
        video_modes[i].yres
      );
    } else {
      printf("%s [%u] %u x %u x %u\n",
        prefix,
        i,
        video_modes[i].xres,
        video_modes[i].yres,
        video_modes[i].bpp
      );
    }
  }

  printf("\n");
}

bool select_video_mode(u8 idx) {
  if (idx >= ARRAY_SIZE(video_modes)) {
    return false;
  }
  selected_video_mode = video_modes[idx].mode;
  return true;
}

void fetch_video_modes(void) {
  if (!vbe_get_info_block(&__ib)) {
    panic("Failed to fetch VBE info block");
  }

  video_mode_append((struct video_mode){
    .mode = 0x3,
    .xres = 80,
    .yres = 25,
    .bpp = 0,
    .is_text_mode = true
  });

  u16 *p = (u16 *)(__ib.VideoModePtr.seg << 4 | __ib.VideoModePtr.off);
  for (; *p != 0xffff; p++) {
    u16 mode = *p;

    if (!vbe_get_mode_info_block(mode, &__mib)) {
      continue;
    }

    if (!is_ok_video_mode(&__mib)) {
      continue;
    }

    if (__mib.XResolution == PREFERRED_RES_X &&
        __mib.YResolution == PREFERRED_RES_Y) {
      selected_video_mode = mode;
    }

    video_mode_append((struct video_mode){
      .mode = mode,
      .xres = __mib.XResolution,
      .yres = __mib.YResolution,
      .bpp = __mib.BitsPerPixel,
      .is_text_mode = __mib.MemoryModel == 0
    });
  }
}
