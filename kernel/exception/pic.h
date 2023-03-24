#ifndef KERNEL_INTS_PIC_H
#define KERNEL_INTS_PIC_H

#include <kernel/common.h>

#define PIC_MASTER_COMMNAD 0x20
#define PIC_MASTER_DATA 0x21
#define PIC_SLAVE_COMMNAD 0xa0
#define PIC_SLAVE_DATA 0xa1

#define PIC_ICW1_INIT 0x10
#define PIC_ICW4_8086 0x01

#define PIC_EOI 0x20

void pic_remap(u8 master_offset, u8 slave_offset);

#endif  // KERNEL_INTS_PIC_H
