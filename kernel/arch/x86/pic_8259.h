#pragma once

#include <tinyos/common/common.h>

void init_pic_8259(u8 master_offset, u8 slave_offset);

void pic_8259_eoi(u8 irq_no);

void pic_8259_mask(u8 irq_no);

void pic_8259_unmask(u8 irq_no);
