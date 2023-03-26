#!/bin/bash

if [ $# -ne 2 ]; then
  exit 1
fi

QEMU_OPTS="-smp 1 -m 256 -no-shutdown -serial file:serial.out -daemonize"
QEMU_DEBUG="-S -gdb tcp::9000"

if [ $2 -eq 1 ]; then
  QEMU_OPTS="${QEMU_OPTS} ${QEMU_DEBUG}"
fi

qemu-system-i386 $QEMU_OPTS $1 2>/dev/null

if [ $2 -eq 1 ]; then
  gdb -q --command=config/gdb
fi

