# make
# insmod vmalloc.ko
# lsmod | grep vmalloc
vmalloc           262144  0
# rmmod vmalloc
# dmesg | tail -n5
[  487.351051] Start vmalloc!
[  487.351292] vmallocmem1 addr = ffff00000b960000
[  487.351702] vmallocmem2 addr = ffff000020760000
[  487.352191] vmallocmem3 addr = ffff000023f60000
[  496.556492] Exit vmalloc!
# make clean
