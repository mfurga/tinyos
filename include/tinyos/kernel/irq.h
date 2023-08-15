#pragma once

#include <tinyos/common/common.h>
#include <tinyos/kernel/hal.h>

typedef void (*irq_handler_t)(int_context_t *ctx);

void init_irq_handling(void);

void irq_hander_register(u8 no, irq_handler_t handler);
