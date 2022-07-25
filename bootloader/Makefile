AS = nasm
ASFLAGS = -fbin

BUILD_DIR = ../build
BOOTLOADER_IMAGE = $(BUILD_DIR)/boot.bin

all:
	$(AS) $(ASFLAGS) -DKERNEL_SIZE=$(KERNEL_SIZE) boot.s -o $(BOOTLOADER_IMAGE)

