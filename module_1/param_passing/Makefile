# Makefile – makefile of real driver
    obj-m += param_passing.o
    obj-m += param_passing_export.o
    KERNEL_SOURCE=/usr/src/linux-headers-4.4.0-148-generic
all:
	make -C $(KERNEL_SOURCE) M=$(PWD) modules
 
clean:
	make -C $(KERNEL_SOURCE) M=$(PWD) clean
