# make
# insmod request_mem_region.ko
# lsmod |grep request_mem_region
request_mem_region        262144  0
# rmmod request_mem_region
# dmesg | tail -n3
[ 1893.484863] Start request mem region!
[ 1893.485170] it's ok for 994115584 .
[ 1914.441594] Exit request_region!
# make clean
