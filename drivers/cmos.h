#pragma once

#include <kernel/common.h>

#define CMOS_REG_RTC_SECOND   0x00
#define CMOS_REG_RTC_MINUTE   0x02
#define CMOS_REG_RTC_HOUR     0x04
#define CMOS_REG_RTC_WEEKDAY  0x06  /* Don't use */
#define CMOS_REG_RTC_DAY      0x07
#define CMOS_REG_RTC_MONTH    0x08
#define CMOS_REG_RTC_YEAR     0x09
#define CMOS_REG_STATUS_A     0x0a
#define CMOS_REG_STATUS_B     0x0b

u8 cmos_read(u8 reg);

void cmos_write(u8 reg, u8 data);

