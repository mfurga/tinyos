#include "common.h"

#define COM1            0x3f8
#define BAUD_RATE_9600  9600

#define DLAB            0x80
#define XMTRDY          0x20

#define TXR             0
#define IER             1
#define FCR             2
#define LCR             3
#define MCR             4
#define LSR             5
#define DLL             0
#define DLH             1

void serial_init(void) {
  outb(COM1 + LCR, 0x3);  /* 8n1 */
  outb(COM1 + IER, 0);    /* no interrupt */
  outb(COM1 + FCR, 0);    /* no fifo */
  outb(COM1 + MCR, 0x3);  /* DTR + RTS */

  u16 divisor = 115200 / BAUD_RATE_9600;
  u8 t = inb(COM1 + LCR);

  outb(COM1 + LCR, t | DLAB);
  outb(COM1 + DLL, divisor & 0xff);
  outb(COM1 + DLH, divisor >> 8);
  outb(COM1 + LCR, t & ~DLAB);
}

void serial_putchar(u8 ch) {
  u16 timeout = 0xffff;
  while ((inb(COM1 + LSR) & XMTRDY) == 0 && --timeout);
  outb(COM1 + TXR, ch);
}
