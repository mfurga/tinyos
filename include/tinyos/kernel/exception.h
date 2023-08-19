#pragma once

#include <tinyos/kernel/hal.h>

typedef void (*exception_handler_t)(int_context_t *ctx);

void init_exception_handling(void);

void exception_handle(int_context_t *ctx);

void exception_handler_register(u8 no, exception_handler_t handler);
