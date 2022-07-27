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

ASFLAGS := -felf
CFLAGS := -Wall -Wextra -std=c11 -O2 -ggdb -ffreestanding -masm=intel
CFLAGS16 := -Wall -Wextra -m16 -std=c11 -Os -ffreestanding -masm=intel

LDFLASGS := -nostdlib -Tlinker.ld

SRCS := $(shell find kernel -name '*.c' -o -name '*.asm')
SRCS16 := $(shell find boot -name '*.c' -o -name '*.asm')

OBJS1 := $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS := $(OBJS1:%.asm=$(BUILD_DIR)/%.o)

OBJS161 := $(SRCS16:%.c=$(BUILD_DIR)/%.o)
OBJS16 := $(OBJS161:%.asm=$(BUILD_DIR)/%.o)

$(KIMAGE): $(OBJS) $(OBJS16)
	@echo $(OBJS)
	@echo $(OBJS16)
	$(LD) $(OBJS) $(OBJS16) -o $@ $(LDFLASGS)
	@nm $@ | awk '{ print $$1 " " $$3 }' >$(BUILD_DIR)/symbols
#	strip $@
	@stat --printf="%s" $@ | awk '{print int(($$0 + 511) / 512)}'>$(BUILD_DIR)/KERNEL_SIZE
	@stat --printf="\033[0;32mKernel size %s bytes\n\033[0m" $@

$(BUILD_DIR)/boot/%.o: boot/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS16) -c $< -o $@

$(BUILD_DIR)/boot/%.o: boot/%.asm
	@mkdir -p $(dir $@)
	$(AS) $(ASFLAGS) $< -o $@

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
	bochs -f config/bochsrc -rc config/bochscmd


