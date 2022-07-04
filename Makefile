
all:
	bootloader/build.sh

hex: all
	hexdump -C boot.bin

run: all
	bochs -f bochsrc

