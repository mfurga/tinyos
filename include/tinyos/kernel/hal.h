#pragma once

#if defined(__i386__)
  #include <tinyos/arch/x86/io.h>
  #include <tinyos/arch/x86/utils.h>
  #include <tinyos/arch/i386/utils.h>

  typedef struct i386_int_context int_context_t;

#elif defined(__x86_64__)
  #include <tinyos/arch/x86/io.h>
  #include <tinyos/arch/x86/utils.h>
  #include <tinyos/arch/x86_64/utils.h>

  typedef struct x86_64_int_context int_context_t;

#else
  #error Unsupported architecture.
#endif

void init_cpu_exception_handling(void);

void init_segmentation(void);
