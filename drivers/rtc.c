#include <drivers/rtc.h>

#define CMOS_ADDRESS_PORT 0x70
#define CMOS_DATA_PORT    0x71

#define CMOS_REG_SECOND   0x00
#define CMOS_REG_MINUTE   0x02
#define CMOS_REG_HOUR     0x04
#define CMOS_REG_WEEKDAY  0x06  /* Don't use */
#define CMOS_REG_DAY      0x07
#define CMOS_REG_MONTH    0x08
#define CMOS_REG_YEAR     0x09
#define CMOS_REG_STATUS_A 0x0a
#define CMOS_REG_STATUS_B 0x0b

#define RTC_DATETIME_CMP(a, b)                                \
  ((a).second != (b).second) || ((a).minute != (b).minute) || \
  ((a).hour != (b).hour) || ((a).day != (b).day) ||           \
  ((a).month != (b).month) || ((a).year != (b).year)

static inline u8 bcd2bin(u8 x) {
  return (x >> 4) * 10 + (x & 0xf);
}

static inline u32 update_in_progress(void) {
  /* FIXME: Add NMI support */
  outb(CMOS_ADDRESS_PORT, CMOS_REG_STATUS_A);
  io_delay();
  return inb(CMOS_DATA_PORT) & 0x80;
}

static inline u8 reg_read(u8 no) {
  /* FIXME: Add NMI support */
  outb(CMOS_ADDRESS_PORT, no);
  io_delay();
  return inb(CMOS_DATA_PORT);
}

rtc_datetime_t rtc_read_datetime(void) {
  rtc_datetime_t last_datetime, datetime;

  while (update_in_progress());

  datetime.second = reg_read(CMOS_REG_SECOND);
  datetime.minute = reg_read(CMOS_REG_MINUTE);
  datetime.hour = reg_read(CMOS_REG_HOUR);
  datetime.day = reg_read(CMOS_REG_DAY);
  datetime.month = reg_read(CMOS_REG_MONTH);
  datetime.year = reg_read(CMOS_REG_YEAR);

  do {
    last_datetime = datetime;
    while (update_in_progress());

    datetime.second = reg_read(CMOS_REG_SECOND);
    datetime.minute = reg_read(CMOS_REG_MINUTE);
    datetime.hour = reg_read(CMOS_REG_HOUR);
    datetime.day = reg_read(CMOS_REG_DAY);
    datetime.month = reg_read(CMOS_REG_MONTH);
    datetime.year = reg_read(CMOS_REG_YEAR);
  } while (RTC_DATETIME_CMP(last_datetime, datetime));

  u8 reg_status_b = reg_read(CMOS_REG_STATUS_B);

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

