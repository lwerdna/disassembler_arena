get qemu: https://download.qemu.org/qemu-8.0.0.tar.xz
./configure
edit qemu-8.0.0/disas/nanomips.c and remove "static" from nanomips_dis()
make
