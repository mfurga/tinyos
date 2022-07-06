all:
	bootloader/build.sh

asm: all
	ndisasm -b 16 boot.img | less

hex: all
	hexdump -C boot.img | less

run: all
	bochs -f bochsrc

