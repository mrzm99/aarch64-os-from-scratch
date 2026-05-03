#/*------------------------------------------------------*/
#/*!
# *      @file   run_qemu.sh
# *      @date   2026.xx.xx
# *      @author mrzm99
# *      @brief  run qemu script
# *      @note
# */
#/*------------------------------------------------------*/
#!/bin/bash

qemu-system-aarch64 \
    -M virt \
    -cpu cortex-a55 \
    -m 4096M \
    -nographic \
    -kernel build/os.elf
