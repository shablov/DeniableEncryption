#!/bin/bash
mkdir -p ./../../lib
make
gcc -shared ./*.o ./*/*.o -o ./../../lib/libarageli.so 


