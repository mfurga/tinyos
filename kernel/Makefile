# Kernel makefile.

AS = nasm
CC = i386-elf-gcc
LD = i386-elf-ld

ASFLAGS = -felf
CFLAGS = -ggdb -ffreestanding
LDFLASGS = -nostdlib -Tlinker.ld

BUILD_DIR = ../build
KERNEL_IMAGE = kernel.bin

SRCS = $(shell find . -name '*.c' -o -name '*.s')
OBJS1 = $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS = $(OBJS1:%.s=$(BUILD_DIR)/%.o)

$(BUILD_DIR)/$(KERNEL_IMAGE): $(OBJS)
	@echo $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLASGS)
	strip $@

	stat --printf="%s" $@ | awk '{print int(($$0 + 511) / 512)}'>$(BUILD_DIR)/KERNEL_SIZE

$(BUILD_DIR)/%.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.s
	mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -r $(BUILD_DIR)

