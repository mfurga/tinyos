#include <tinyos/kernel/hal.h>
#include <tinyos/kernel/panic.h>
#include <tinyos/kernel/timer.h>

#define PIT_FREQ            1193180

#define PIT_CH0_PORT        0x40  /* [RW] Channel 0 data port */
#define PIT_CH1_PORT        0x41  /* [RW] Channel 2 data port */
#define PIT_CH2_PORT        0x42  /* [RW] Channel 3 data port */
#define PIT_CMD_PORT        0x43  /* [ W] Mode/Command register port */

#define PIT_MODE_BIN        0b00000000
#define PIT_MODE_BCD        0b00000001

#define PIT_OPMODE_0        0b00000000  /* Interrupt on terminal count */
#define PIT_OPMODE_1        0b00000010  /* Hardware re-triggerable one-shot */
#define PIT_OPMODE_2        0b00000100  /* Rate generator */
#define PIT_OPMODE_3        0b00000110  /* Square wave generator */
#define PIT_OPMODE_4        0b00001000  /* Software triggered strobe */
#define PIT_OPMODE_5        0b00010100  /* hardware triggered strobe */

#define PIT_ACC_LATCH       0b00000000  /* Latch count value command */
#define PIT_ACC_LO          0b00100000  /* Access low byte only */
#define PIT_ACC_HI          0b01000000  /* Access high byte only */
#define PIT_ACC_LOHI        0b01100000  /* Access low and high byte */

#define PIT_CH0             0b00000000  /* Channel 0 */
#define PIT_CH1             0b01000000  /* Channel 1 */
#define PIT_CH2             0b10000000  /* Channel 2 */
#define PIT_RB_CMD          0b11000000  /* Read-back command (8254 only) */

u64 hw_timer_init(u32 freq) {
  u32 divisor = PIT_FREQ / freq;
  assert(divisor <= UINT16_MAX);

  outb(PIT_CMD_PORT, PIT_MODE_BIN |
                     PIT_OPMODE_3 |
                     PIT_ACC_LOHI |
                     PIT_CH0);

  outb(PIT_CH0_PORT, divisor & 0xff);
  outb(PIT_CH0_PORT, (divisor >> 8) & 0xff);

  /* Calculate the real interval between ticks. */
  u64 real_interval = PIT_FREQ / freq;
  real_interval *= TIMER_SCALE;
  real_interval /= PIT_FREQ;

  return real_interval;
}
