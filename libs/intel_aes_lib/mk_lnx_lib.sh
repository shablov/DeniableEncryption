#!/bin/bash
sz=64
mkdir -p obj/
yasm="./yasm"
asm="iaesx do_rdtsc"
for i in $asm; do $yasm -D__linux__ -g dwarf2 -f elf${sz} asm/$i.s -o obj/$i.o; done
gcc -fPIC -O3 -g -m${sz} -Iinclude/ -c src/intel_aes.c -o obj/intel_aes.o
mkdir -p lib
gcc -shared -o ./lib/libintel_aes.so obj/*


