#!/bin/bash

BASE=$(dirname -- "$0";)

echo "[*] Building stage1 ..."
nasm -DSTAGE2_SIZE=1 $BASE/boot_stage1.asm -o $BASE/boot_stage1.bin
echo "[*] Building stage2 ..."
nasm $BASE/boot_stage2.asm -o $BASE/boot_stage2.bin

cat $BASE/boot_stage1.bin >boot.bin
cat $BASE/boot_stage2.bin >>boot.bin

rm -f $BASE/boot_stage1.bin $BASE/boot_stage2.bin
echo "[+] Done."

