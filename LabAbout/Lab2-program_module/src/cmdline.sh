make clean
make

insmod module1.ko
insmod module2.ko int_var=666 str_var=hello int_array=10,20,30,40
insmod module3.ko
insmod module4.ko
lsmod | grep module

cat /proc/hello_proc
cat /proc/hello_dir/hello
echo nice > /proc/hello_dir/hello
cat /proc/hello_dir/hello

rmmod module4.ko
rmmod module3.ko
rmmod module2.ko
rmmod module1.ko

clear
dmesg | tail -n80
