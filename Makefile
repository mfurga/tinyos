# Kernel makefile.

ROOT_DIR := $(shell dirname $(realpath $(firstword $(MAKEFILE_LIST))))

VERBOSE := 0
DEBUG := 0

BUILD_DIR := build
BUILD_DIR := $(ROOT_DIR)/$(BUILD_DIR)
KIMAGE := $(BUILD_DIR)/kernel.elf
BIMAGE := $(BUILD_DIR)/bootloader/boot.img
OS_IMAGE := $(BUILD_DIR)/tinyos.img

ifeq ($(VERBOSE), 1)
	Q :=
else
	Q := @
endif

ifeq ($(DEBUG), 1)
	D := -ggdb
else
	D := 
endif

CROSS_COMPILE := i386-elf

AS := $(CROSS_COMPILE)-gcc
CC := $(CROSS_COMPILE)-gcc
LD := $(CROSS_COMPILE)-ld
NM := $(CROSS_COMPILE)-nm
STRIP := $(CROSS_COMPILE)-strip
OBJCOPY := $(CROSS_COMPILE)-objcopy
OBJDUMP := $(CROSS_COMPILE)-objdump

ASFLAGS := $(D) -Iinclude -Iinclude/3rd_party -c
CFLAGS := $(D) -Wall -Wextra -Wno-array-bounds -std=c11 -O2 -Iinclude -Iinclude/3rd_party -ffreestanding -masm=intel

LDFLASGS := -lgcc -static -nostdlib -Tlinker.ld

SRCS := $(shell find * -type f -name '*.[c|S]' -not -path 'bootloader/*')
FONTS := $(shell find * -type f -name '*.psf' -not -path 'bootloader/*')

OBJS1 := $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS1 += $(FONTS:%.psf=$(BUILD_DIR)/%.o)
OBJS := $(OBJS1:%.S=$(BUILD_DIR)/%.o)

all: $(BIMAGE) $(KIMAGE)
	@cat $(BIMAGE) > $(OS_IMAGE)
	@cat $(KIMAGE) >> $(OS_IMAGE)
	@truncate -s 8M $(OS_IMAGE)
	@printf "\033[0;32mOS image created!\n\033[0m"

$(BIMAGE): $(KIMAGE)
	@cd bootloader && cat $(BUILD_DIR)/KERNEL_SIZE | $(MAKE) KERNEL_SIZE=$$(xargs)

$(KIMAGE): $(OBJS)
	@echo "Linking ..."
	$(Q)$(CC) $(OBJS) -o $@ $(LDFLASGS)
	@$(NM) $@ | awk '{ print $$1 " " $$3 }' >$(BUILD_DIR)/symbols
ifeq ($(DEBUG), 0)
	@$(STRIP) $@
endif
	@stat --printf="%s" $@ | awk '{print int(($$0 + 511) / 512)}'>$(BUILD_DIR)/KERNEL_SIZE
	@stat --printf="\033[0;32mKernel size %s bytes\n\033[0m" $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.S
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	$(Q)$(AS) $(ASFLAGS) $< -o $@

$(BUILD_DIR)/%.o: %.psf
	@mkdir -p $(dir $@)
	@echo "Converting font $^"
	$(Q)cd $(shell dirname $<) && $(OBJCOPY) -O elf32-i386 -I binary $(shell basename $<) $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

.PHONY: asm
asm: all
	$(OBJDUMP) -d $(KIMAGE) -Mintel

.PHONY: run
run: all
	#./tools/run_qemu.sh $(KIMAGE) $(DEBUG)
	bochs -f tools/bochsrc -rc tools/bochs_commands

