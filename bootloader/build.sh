#!/bin/bash

BASE=$(dirname -- "$0";)

echo "[*] Building stage1 ..."
nasm -DSTAGE2_SIZE=1 $BASE/stage1.asm -o $BASE/stage1.img
echo "[*] Building stage2 ..."
nasm $BASE/stage2.asm -o $BASE/stage2.img

cat $BASE/stage1.img >boot.img
cat $BASE/stage2.img >>boot.img

rm -f $BASE/stage1.img $BASE/stage2.img
echo "[+] Done."

