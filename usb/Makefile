# Makefile – makefile of real driver
    obj-m += usb_ms.o
    
    KERNEL_SOURCE=/usr/src/linux-headers-4.4.0-148-generic
all:
	make -C $(KERNEL_SOURCE) M=$(PWD) modules
 
clean:
	make -C $(KERNEL_SOURCE) M=$(PWD) clean
