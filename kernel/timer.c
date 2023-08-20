#include <tinyos/kernel/timer.h>
#include <tinyos/kernel/irq.h>
#include <tinyos/kernel/printk.h>

#define TIMER_FREQ 500  /* 500 Hz */

static u64 ticks;  /* ticks since the timer started */
static u64 ticks_interval;  /* time in ns between ticks  */
static u64 time;  /* time in ns sicne the timer started */

static void timer_handler(int_context_t *ctx) {
  UNUSED(ctx);
  ticks++;
  time += ticks_interval;
}

u64 timer_get_time(void) {
  return time;
}

u64 timer_get_ticks(void) {
  return ticks;
}

void init_timer(void) {
  ticks_interval = hw_timer_init(TIMER_FREQ);

  irq_hander_register(X86_IRQ_PC_TIMER, &timer_handler);
}
