#!/bin/bash
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

echo "Building Booting Process ->"   
echo -e "____________________________\n"

make build/boot/mbr/bootsec.bin

make build/boot/stage2/loader.bin

echo -e "\n==================================================================\n"

echo "Building Kernel ->"
echo -e "___________________\n"

echo "Generating isrs_gens ->"
echo -e "___________________\n"

./build_scripts/generate_isrs.sh src/kernel/arch/x86/isr/isrs_gen.c src/kernel/include/arch/x86/isr/isrs_gen.inc

make build/kernel/kernel.bin

echo -e "\n==================================================================\n"

echo "Building Pre-Boot CRC-32 Calculation ->"   
echo -e "________________________________________\n"

make build/boot/stage2/include/preboot_crc32_calc.elf

echo -e "\nCalculating Pre-Boot Kernel CRC-32 Checksum ->"
echo -e "_______________________________________________\n"

RESULT=$(./build/boot/stage2/include/preboot_crc32_calc.elf)
 
echo "Pre-Boot Kernel's CRC-32 Checksum Result: $RESULT"

echo -n $RESULT | xxd -p -r > build/boot/stage2/crc-32/preboot_crc32_result.bin

echo -e "\n==================================================================\n"

echo "Building Disk Image ->"   
echo -e "_______________________\n"

make build/disk.img