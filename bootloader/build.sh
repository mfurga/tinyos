#!/bin/bash

BASE=$(dirname -- "$0";)

echo "[*] Building stage2 ..."
nasm $BASE/stage2.asm -o $BASE/stage2.img

STAGE2_SIZE=$(stat --printf="%s" $BASE/stage2.img)

echo "[*] Building stage1 ..."
nasm -DSTAGE2_SIZE=$(($STAGE2_SIZE / 512)) $BASE/stage1.asm -o $BASE/stage1.img

cat $BASE/stage1.img >boot.img
cat $BASE/stage2.img >>boot.img

rm -f $BASE/stage1.img $BASE/stage2.img
echo "[+] Done."

