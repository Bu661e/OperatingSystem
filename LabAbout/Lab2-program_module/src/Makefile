obj-m := module1.o module2.o module3.o module4.o
KDIR := /root/kernel
PWD := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm *.o *.ko *.mod *.mod.c Module.symvers modules.order -f
