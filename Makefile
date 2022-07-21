CC = i386-elf-gcc
LD = i386-elf-ld
CFLAGS = -ggdb -ffreestanding
LDFLASGS = -nostdlib --oformat binary -Ttext 0x0

BUILD_DIR = build

BOOTLOADER_SRC = bootloader
BOOTLOADER_IMAGE = boot.bin

KERNEL_SRC = kernel
KERNEL_IMAGE = kernel.bin

OS_IMAGE = os.bin

SRCS = $(shell find $(KERNEL_SRC) -name '*.c')
OBJS = $(SRCS:%=$(BUILD_DIR)/%.o)

all: $(BUILD_DIR)/$(KERNEL_IMAGE)
	$(eval KERNEL_SIZE = $(shell stat --printf="%s" $(BUILD_DIR)/$(KERNEL_IMAGE)))
	$(eval KERNEL_SIZE = $(shell echo $$(( ($(KERNEL_SIZE) + 511) / 512 ))))

	mkdir -p $(BUILD_DIR)/$(BOOTLOADER_SRC)
	nasm -DKERNEL_SIZE=$(KERNEL_SIZE) $(BOOTLOADER_SRC)/stage1.asm -o $(BUILD_DIR)/$(BOOTLOADER_SRC)/stage1.bin
	nasm $(BOOTLOADER_SRC)/stage2.asm -o $(BUILD_DIR)/$(BOOTLOADER_SRC)/stage2.bin

	cat $(BUILD_DIR)/$(BOOTLOADER_SRC)/stage1.bin > $(BUILD_DIR)/$(OS_IMAGE)
	cat $(BUILD_DIR)/$(BOOTLOADER_SRC)/stage2.bin >> $(BUILD_DIR)/$(OS_IMAGE)
	cat $(BUILD_DIR)/$(KERNEL_IMAGE) >> $(BUILD_DIR)/$(OS_IMAGE)

$(BUILD_DIR)/$(KERNEL_IMAGE): $(OBJS)
	$(LD) $(OBJS) -o $@ $(LDFLASGS)

$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -r $(BUILD_DIR)

#asm: all
#	ndisasm -b 16 boot.img | less

#hex: all
#	hexdump -C boot.img | less

run: all
	bochs -f bochsrc

