### aarch64-os-from-scratch

This project is a record of building an operating system from scratch to run on QEMU.  

- Launguage: C99
- Build System: Make
- arch: Armv8 (AArch64)

scripts directory includes start script.
To start with GDB is following. 
```
start QEMU terminal
$ ./scripts/run_qemu_with_gdb.sh

another terminal
$ gdb-multiarch build/os.elf
(gdb) target remote localhost:1234
```

