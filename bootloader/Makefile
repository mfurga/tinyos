# Bootloader makefile.

SHELL := /bin/bash

VERBOSE := 0

BUILD_DIR := ../build/bootloader

STAGE1 := $(BUILD_DIR)/stage1.img
STAGE2 := $(BUILD_DIR)/stage2.img
BIMAGE := $(BUILD_DIR)/boot.img

ifeq ($(VERBOSE), 1)
	Q :=
else
	Q := @
endif

CROSS_COMPILE := i386-elf

AS := nasm
CC := $(CROSS_COMPILE)-gcc
LD := $(CROSS_COMPILE)-ld
NM := $(CROSS_COMPILE)-nm
STRIP := $(CROSS_COMPILE)-strip
OBJCOPY := $(CROSS_COMPILE)-objcopy
OBJDUMP := $(CROSS_COMPILE)-objdump

ASFLAGS := -felf32 -i.
CFLAGS := -Wall -Wextra -std=c11 -O2 -I. -ggdb -ffreestanding -masm=intel

LDFLASGS := -static -nostdlib -Tlinker.ld

STAGE1_SRC := boot/stage1.asm
SRCS := $(shell find * -type f -name '*.c' -o -name '*.asm' -not -path 'boot/*')

OBJS1 := $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS := $(OBJS1:%.asm=$(BUILD_DIR)/%.o)

.PHONY: all
all: $(STAGE2) $(STAGE1)
	@cat $(STAGE1) > $(BIMAGE)
	@cat $(STAGE2) >> $(BIMAGE)
	@printf "\033[0;32mBootloader image created!\n\033[0m"

$(STAGE1): $(STAGE1_SRC)
	@mkdir -p $(BUILD_DIR)
	@echo "Compiling stage1.asm"
	@mapfile -t <$(BUILD_DIR)/STAGE2_SIZE && $(AS) -fbin -DSTAGE2_SIZE=$$MAPFILE $(STAGE1_SRC) -o $(STAGE1)

$(STAGE2): $(OBJS)
	@echo "Linking ..."
	$(Q)$(LD) $(OBJS) -o $(BUILD_DIR)/stage2.elf $(LDFLASGS)
	@$(NM) $(BUILD_DIR)/stage2.elf | awk '{ print $$1 " " $$3 }' >$(BUILD_DIR)/symbols
	@$(STRIP) $(BUILD_DIR)/stage2.elf
	@$(OBJCOPY) -O binary $(BUILD_DIR)/stage2.elf $@
	@dd if=/dev/zero bs=1 count=$$(stat --printf="%s" $@ | awk '{print int((512 - $$0 % 512))}') >>$@ 2>/dev/null
	@stat --printf="%s" $@ | awk '{print int(($$0 + 511) / 512)}'>$(BUILD_DIR)/STAGE2_SIZE
	@stat --printf="\033[0;32mBootloader stage 2 size: %s bytes\n\033[0m" $@

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	$(Q)$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.asm
	@mkdir -p $(dir $@)
	@echo "Compiling $^"
	$(Q)$(AS) $(ASFLAGS) $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

.PHONY: run
run: all
	bochs -f config/bochsrc -rc config/bochscmd

