# Kernel makefile.

BUILD_DIR := build
BIMAGE := $(BUILD_DIR)/boot.img
KIMAGE := $(BUILD_DIR)/kernel.elf
OS_IMAGE = $(BUILD_DIR)/os.img

all: $(BIMAGE) $(KIMAGE)
	cat $(BIMAGE) > $(OS_IMAGE)
	cat $(KIMAGE) >> $(OS_IMAGE)
	@printf "\033[0;32mOS image created!\n\033[0m"

$(BIMAGE): $(KIMAGE)
	cd bootloader && cat ../$(BUILD_DIR)/KERNEL_SIZE | $(MAKE) KERNEL_SIZE=$$(xargs)

AS := nasm
CC := i386-elf-gcc
LD := i386-elf-ld

ASFLAGS := -felf32 -iarch -i.
CFLAGS := -Wall -Wextra -std=c11 -O2 -I. -ggdb -ffreestanding -masm=intel

LDFLASGS := -static -nostdlib -Tlinker.ld

SRCS := $(shell find * -type f -name '*.c' -o -name '*.asm' -not -path 'bootloader/*')

OBJS1 := $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS := $(OBJS1:%.asm=$(BUILD_DIR)/%.o)

$(KIMAGE): $(OBJS)
	@echo $(OBJS)
	$(LD) $(OBJS) $(OBJS16) -o $@ $(LDFLASGS)
	@nm $@ | awk '{ print $$1 " " $$3 }' >$(BUILD_DIR)/symbols
	strip $@
	@stat --printf="%s" $@ | awk '{print int(($$0 + 511) / 512)}'>$(BUILD_DIR)/KERNEL_SIZE
	@stat --printf="\033[0;32mKernel size %s bytes\n\033[0m" $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

clean:
	rm -r $(BUILD_DIR)

asm16: all
	objdump -d $(KIMAGE) -Mintel -mi8086

asm: all
	objdump -d $(KIMAGE) -Mintel

run: all
	dd if=/dev/zero of=build/test.img count=1008 bs=512
	dd if=build/os.img of=build/test.img conv=notrunc
	mv build/test.img build/os.img
	bochs -f config/bochsrc -rc config/bochscmd

