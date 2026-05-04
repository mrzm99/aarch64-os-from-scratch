#/*------------------------------------------------------*/
#/*!
# *      @file   run_qemu_with_gdb.sh
# *      @date   2026.xx.xx
# *      @author mrzm99
# *      @brief  run qemu with GDB script
# *      @note
# */
#/*------------------------------------------------------*/
#!/bin/bash

qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a55 \
    -m 1024M \
    -nographic \
    -kernel build/os.elf \
    -s -S
