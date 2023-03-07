#pragma once

#include <kernel/ints/int_handlers.h>

void NORETURN kernel_panic(const int_context_t *context);

