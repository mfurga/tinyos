# Makefile

BUILD_DIR = build
BOOTLOADER_IMAGE = $(BUILD_DIR)/boot.bin
KERNEL_IMAGE = $(BUILD_DIR)/kernel.bin
OS_IMAGE = $(BUILD_DIR)/os.bin

all:
	cd kernel && $(MAKE)
	cd bootloader && cat ../$(BUILD_DIR)/KERNEL_SIZE | $(MAKE) KERNEL_SIZE=$$(xargs)

	cat $(BOOTLOADER_IMAGE) > $(OS_IMAGE)
	cat $(KERNEL_IMAGE) >> $(OS_IMAGE)

	@echo "Done."

clean:
	rm -r $(BUILD_DIR)

run: all
	bochs -f bochsrc

