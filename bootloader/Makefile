AS = nasm
ASFLAGS = -fbin

BUILD_DIR = ../build
BIMAGE = $(BUILD_DIR)/boot.img

.PHONY: all
all:
	@echo "Compiling boot.asm"
	@$(AS) $(ASFLAGS) -DKERNEL_SIZE=$(KERNEL_SIZE) boot.asm -o $(BIMAGE)

