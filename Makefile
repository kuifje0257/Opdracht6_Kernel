obj-m += kernel_ZP.o
obj-m += kernel_MP.o
obj-m += kernel_MPT.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) clean
