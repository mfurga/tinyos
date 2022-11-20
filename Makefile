# Kernel makefile.

VERBOSE := 0

BUILD_DIR := build
BIMAGE := $(BUILD_DIR)/boot.img
KIMAGE := $(BUILD_DIR)/kernel.elf
OS_IMAGE = $(BUILD_DIR)/os.img

ifeq ($(VERBOSE), 1)
	Q :=
else
	Q := @
endif

CROSS_COMPILE := i386-elf

AS := nasm
CC := $(CROSS_COMPILE)-gcc
LD := $(CROSS_COMPILE)-ld

ASFLAGS := -felf32 -i.
CFLAGS := -Wall -Wextra -std=c11 -O2 -I. -ggdb -ffreestanding -masm=intel

LDFLASGS := -static -nostdlib -Tlinker.ld

SRCS := $(shell find * -type f -name '*.c' -o -name '*.asm' -not -path 'bootloader/*')

OBJS1 := $(SRCS:%.c=$(BUILD_DIR)/%.o)
OBJS := $(OBJS1:%.asm=$(BUILD_DIR)/%.o)

all: $(BIMAGE) $(KIMAGE)
	@cat $(BIMAGE) > $(OS_IMAGE)
	@cat $(KIMAGE) >> $(OS_IMAGE)
	@printf "\033[0;32mOS image created!\n\033[0m"

$(BIMAGE): $(KIMAGE)
	@cd bootloader && cat ../$(BUILD_DIR)/KERNEL_SIZE | $(MAKE) KERNEL_SIZE=$$(xargs)

$(KIMAGE): $(OBJS)
	@echo "Linking $^"
	$(Q)$(LD) $(OBJS) -o $@ $(LDFLASGS)
	@nm $@ | awk '{ print $$1 " " $$3 }' >$(BUILD_DIR)/symbols
	@strip $@
	@stat --printf="%s" $@ | awk '{print int(($$0 + 511) / 512)}'>$(BUILD_DIR)/KERNEL_SIZE
	@stat --printf="\033[0;32mKernel size %s bytes\n\033[0m" $@

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

.PHONY: asm
asm: all
	objdump -d $(KIMAGE) -Mintel

.PHONY: run
run: all
	dd if=build/os.img of=build/test.img conv=notrunc
	mv build/test.img build/os.img
	bochs -f config/bochsrc -rc config/bochscmd

