#include <tinyos/drivers/rtc.h>
#include <tinyos/drivers/cmos.h>

#define RTC_DATETIME_CMP(a, b)                                \
  ((a).second != (b).second) || ((a).minute != (b).minute) || \
  ((a).hour != (b).hour) || ((a).day != (b).day) ||           \
  ((a).month != (b).month) || ((a).year != (b).year)

static inline u8 bcd2bin(u8 x) {
  return (x >> 4) * 10 + (x & 0xf);
}

static inline u32 update_in_progress(void) {
  return cmos_read(CMOS_REG_STATUS_A) & 0x80;
}

rtc_datetime_t rtc_read_datetime(void) {
  rtc_datetime_t last_datetime, datetime;

  while (update_in_progress());

  datetime.second = cmos_read(CMOS_REG_RTC_SECOND);
  datetime.minute = cmos_read(CMOS_REG_RTC_MINUTE);
  datetime.hour = cmos_read(CMOS_REG_RTC_HOUR);
  datetime.day = cmos_read(CMOS_REG_RTC_DAY);
  datetime.month = cmos_read(CMOS_REG_RTC_MONTH);
  datetime.year = cmos_read(CMOS_REG_RTC_YEAR);

  do {
    last_datetime = datetime;

    while (update_in_progress());

    datetime.second = cmos_read(CMOS_REG_RTC_SECOND);
    datetime.minute = cmos_read(CMOS_REG_RTC_MINUTE);
    datetime.hour = cmos_read(CMOS_REG_RTC_HOUR);
    datetime.day = cmos_read(CMOS_REG_RTC_DAY);
    datetime.month = cmos_read(CMOS_REG_RTC_MONTH);
    datetime.year = cmos_read(CMOS_REG_RTC_YEAR);

  } while (RTC_DATETIME_CMP(last_datetime, datetime));

  u8 reg_status_b = cmos_read(CMOS_REG_STATUS_B);

  /* Convert BCD to binary. */
  if (!(reg_status_b & 0x04)) {
    datetime.second = bcd2bin(datetime.second);
    datetime.minute = bcd2bin(datetime.minute);
    datetime.hour = (((datetime.hour >> 4) & 0x7) * 10 + \
                     (datetime.hour & 0xf)) | (datetime.hour & 0x80);
    datetime.day = bcd2bin(datetime.day);
    datetime.month = bcd2bin(datetime.month);
    datetime.year = bcd2bin(datetime.year);
  }

  /* Convert to 24 hour format. */
  if (!(reg_status_b & 0x02) && datetime.hour & 0x80) {
    datetime.hour = ((datetime.hour & 0x7f) + 12) % 24;
  }

  /* TODO: Add century register support. */

  return datetime;
}

