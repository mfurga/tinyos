#pragma once

#include <tinyos/common/common.h>

typedef struct {
  u8 second;
  u8 minute;
  u8 hour;
  u8 day;
  u8 month;
  u8 year;
} rtc_datetime_t;

rtc_datetime_t rtc_read_datetime(void);

