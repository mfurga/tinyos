#include <tinyos/common/common.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/printk.h>
#include <tinyos/common/string.h>
#include <tinyos/common/colors.h>

#include <multiboot.h>

#include "fb.h"

static struct fb fb;
static struct fb_font fb_font;
static u32 fb_colors[16];

static u32 make_color(u8 r, u8 g, u8 b) {
  return ((r << fb.red_pos) & fb.red_mask) |
         ((g << fb.green_pos) & fb.green_mask) |
         ((b << fb.blue_pos) & fb.blue_mask);
}

static void fb_init_colors(void) {
  u32 *c = fb_colors;

  c[COLOR_BLACK]        = make_color(0x00, 0x00, 0x00);
  c[COLOR_BLUE]         = make_color(0x00, 0x00, 0xaa);
  c[COLOR_GREEN]        = make_color(0x00, 0xaa, 0x00);
  c[COLOR_CYAN]         = make_color(0x00, 0xaa, 0xaa);
  c[COLOR_RED]          = make_color(0xaa, 0x00, 0x00);
  c[COLOR_PURPLE]       = make_color(0xaa, 0x00, 0xaa);
  c[COLOR_BROWN]        = make_color(0xaa, 0x55, 0x00);
  c[COLOR_GRAY]         = make_color(0xaa, 0xaa, 0xaa);
  c[COLOR_DARK_GRAY]    = make_color(0x55, 0x55, 0x55);
  c[COLOR_LIGHT_BLUE]   = make_color(0x55, 0x55, 0xff);
  c[COLOR_LIGHT_GREEN]  = make_color(0x55, 0xff, 0x55);
  c[COLOR_LIGHT_CYAN]   = make_color(0x55, 0xff, 0xff);
  c[COLOR_LIGHT_RED]    = make_color(0xff, 0x55, 0x55);
  c[COLOR_LIGHT_PURPLE] = make_color(0xff, 0x55, 0xff);
  c[COLOR_YELLOW]       = make_color(0xff, 0xff, 0x55);
  c[COLOR_WHITE]        = make_color(0xff, 0xff, 0xff);
}

#define PSF1_MAGIC 0x0436
#define PSF2_MAGIC 0x864ab572

static void fb_init_font(void *f) {
  struct {
    u16 magic;
    u8 font_mode;
    u8 glyph_size;
  } PACKED *h1 = f;

  struct {
    u32 magic;
    u32 version;
    u32 header_size;
    u32 flags;
    u32 length;
    u32 glyph_size;
    u32 height;
    u32 width;
  } PACKED *h2 = f;

  if (h1->magic == PSF1_MAGIC) {
    fb_font.width = 8;
    fb_font.width_bytes = 1;
    fb_font.height = h1->glyph_size;
    fb_font.glyph_size = h1->glyph_size;
    fb_font.glyph_data = (u8 *)h1 + sizeof(*h1);
  } else {
    assert(h2->magic == PSF2_MAGIC);
    assert(h2->version == 0);
    assert(h2->header_size == 32);

    fb_font.width = h2->width;
    fb_font.width_bytes = h2->glyph_size / h2->height;
    fb_font.height = h2->height;
    fb_font.glyph_size = h2->glyph_size;
    fb_font.glyph_data = (u8 *)h2 + h2->header_size;
  }
}

void fb_draw_pixel(u32 x, u32 y, u32 color) {
  assert(x < fb.width);
  assert(y < fb.height);

  if (likely(fb.bpp == 32)) {
    *(volatile u32 *)(fb.paddr + (y * fb.pitch) + (x << 2)) = color;
  } else {
    /* Assume 24. */
    assert(fb.bpp == 24);
    memcpy((void *)(fb.paddr + y * fb.pitch + x * 3), &color, 3);
  }
}

#define GET_BIT(x, p) (((x) >> (p)) & 1)

void fb_draw_char(u32 x, u32 y, u16 e) {
  u8 c = VGA_ENTRY_CHAR(e);
  u8 fg = VGA_ENTRY_FG(e);
  u8 bg = VGA_ENTRY_BG(e);

  u8 *glyph = &fb_font.glyph_data[fb_font.glyph_size * c];

  for (u32 h = 0; h < fb_font.height; h++) {
    for (u32 w = 0; w < fb_font.width; w++) {
      u32 off = h * fb_font.width_bytes + w / 8;

      if (GET_BIT(glyph[off], 7 - (w % 8))) {
        fb_draw_pixel(x + w, y + h, fb_colors[fg]);
      } else {
        fb_draw_pixel(x + w, y + h, fb_colors[bg]);
      }
    }
  }
}

void setup_fb_from_multiboot(struct multiboot_info *mbi) {
  extern char _binary_font16x32_psf_start;
  extern char _binary_font8x16_psf_start;

  assert(mbi->framebuffer_addr <= UINT32_MAX);

  fb.paddr = (u32)mbi->framebuffer_addr;
  fb.width = mbi->framebuffer_width;
  fb.height = mbi->framebuffer_height;
  fb.bpp = mbi->framebuffer_bpp;
  fb.pitch = mbi->framebuffer_pitch;
  if (fb.pitch == 0) {
    fb.pitch = fb.width * (fb.bpp / 8);
  }

  switch (mbi->framebuffer_type) {
    case MULTIBOOT_FRAMEBUFFER_TYPE_INDEXED:
      printk("Indexed framebuffer is not supported\n");
      return;

    case MULTIBOOT_FRAMEBUFFER_TYPE_EGA_TEXT:
      printk("Text mode framebuffer is not supported\n");
      return;

    case MULTIBOOT_FRAMEBUFFER_TYPE_RGB:
      #define bitmask(s, p) (((1u << (s)) - 1) << (p)) 

      fb.red_pos = mbi->framebuffer_red_field_position;
      fb.red_mask = bitmask(mbi->framebuffer_red_mask_size, fb.red_pos);
      fb.green_pos = mbi->framebuffer_green_field_position;
      fb.green_mask = bitmask(mbi->framebuffer_green_mask_size, fb.green_pos);
      fb.blue_pos = mbi->framebuffer_blue_field_position;
      fb.blue_mask = bitmask(mbi->framebuffer_blue_mask_size, fb.blue_pos);

      #undef bitmask
    break;

    default:
      printk("Invalid framebuffer type\n");
      return;
  }

  fb_init_colors();
  fb_init_font(&_binary_font8x16_psf_start);

  printk("Framebuffer info:\n"
         "paddr: 0x%08x\n"
         "resolution: %u x %u x %u\n"
         "pitch: %u\n"
         "colors: red (%u %u) green (%u %u) blue (%u %u)\n",
         fb.paddr, fb.width, fb.height, fb.bpp, fb.pitch,
         fb.red_pos, fb.red_mask,
         fb.green_pos, fb.green_mask,
         fb.blue_pos, fb.blue_mask);
}
