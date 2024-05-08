# make
# insmod request_region.ko
# lsmod |grep request_region
request_region        262144  0
# rmmod request_region
# dmesg | tail -n4
[  496.556492] Exit vmalloc!
[ 1578.353837] Start request region!
[ 1578.354111] it's ok for 22222 .
[ 1646.519287] Exit request_region!
# make clean
