#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="build"
ISO_DIR="${BUILD_DIR}/isodir"

mkdir -p "${ISO_DIR}/boot/grub"

echo "[shark] assembling boot.s..."
as -32 boot.s -o "${BUILD_DIR}/boot.o"

echo "[shark] compiling kernel.c..."
gcc -m32 -c src/kernel.c -o "${BUILD_DIR}/kernel.o" -std=gnu99 -ffreestanding -O2 -Wall -Wextra

echo "[shark] linking kernel..."
gcc -m32 -T linker.ld -o "${BUILD_DIR}/shark.bin" -ffreestanding -O2 -nostdlib -no-pie "${BUILD_DIR}/kernel.o" "${BUILD_DIR}/boot.o"

echo "[shark] verifying multiboot..."
grub-file --is-x86-multiboot "${BUILD_DIR}/shark.bin"

echo "[shark] building ISO..."
cp "${BUILD_DIR}/shark.bin" "${ISO_DIR}/boot/shark.bin"
cp grub.cfg "${ISO_DIR}/boot/grub/grub.cfg"
grub-mkrescue -o "${BUILD_DIR}/shark.iso" "${ISO_DIR}"

echo "[shark] launching QEMU..."
qemu-system-x86_64 -cdrom "${BUILD_DIR}/shark.iso"
