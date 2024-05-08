# make
# insmod kmalloc.ko
# lsmod | grep kmalloc
kmalloc           262144  0
# rmmod kmalloc
# dmesg | tail -n6
[   44.653995] kmalloc: loading out-of-tree module taints kernel.
[   44.654464] kmalloc: module verification failed: signature and/or required key missing - tainting kernel
[   44.655688] Start kmalloc!
[   44.655879] kmallocmem1 addr = ffff800171e53000
[   44.656181] kmallocmem2 addr = ffff8001704fe000
[  106.904519] Exit kmalloc!
# make clean
