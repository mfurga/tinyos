#include <kernel/exception/handler.h>
#include <kernel/pit.h>

#define PIT_CHANNEL_0_PORT 0x40
#define PIT_CHANNEL_1_PORT 0x41
#define PIT_CHANNEL_2_PORT 0x42
#define PIT_COMMAND_PORT 0x43

#define PIT_BINMODE 0
#define PIT_BCDMODE 1

#define PIT_OPMODE_0 (0b000 << 1)
#define PIT_OPMODE_1 (0b001 << 1)
#define PIT_OPMODE_2 (0b010 << 1)
#define PIT_OPMODE_3 (0b011 << 1)
#define PIT_OPMODE_4 (0b100 << 1)
#define PIT_OPMODE_5 (0b101 << 1)

#define PIT_ACCMODE_LATCH (0b00 << 4)
#define PIT_ACCMODE_LO    (0b01 << 4)
#define PIT_ACCMODE_HI    (0b10 << 4)
#define PIT_ACCMODE_LOHI  (0b11 << 4)

#define PIT_CHANNEL_0 (0b00 << 6)
#define PIT_CHANNEL_1 (0b01 << 6)
#define PIT_CHANNEL_2 (0b10 << 6)

static u32 counter;

static void pit_handler(int_context_t *context) {
  UNUSED(context);
  counter++;
}

void pit_init(u32 freq) {
  int_handler_register(IRQ0, &pit_handler);

  u32 divisor = 1193180 / freq;

  outb(PIT_COMMAND_PORT, PIT_BINMODE |
                         PIT_OPMODE_3 |
                         PIT_ACCMODE_LOHI |
                         PIT_CHANNEL_0);

  u8 lo = (u8)(divisor & 0xff);
  u8 hi = (u8)((divisor >> 8) & 0xff);

  outb(PIT_CHANNEL_0_PORT, lo);
  outb(PIT_CHANNEL_0_PORT, hi);
}

