# make
# insmod hello_world.ko guy="Dinu" year=2013
# lsmod | grep hello
hello_world           262144  0
# rmmod hello_world
# dmesg | tail -n3
[ 6228.122161] Init module.
[ 6228.122433] Hello, Dinu, 2013!
[ 6254.880206] Exit module.
# make clean