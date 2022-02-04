obj-m += dyn_allocating.o
KDIR = /lib/modules/$(shell uname -r)/build

all:
	make -C $(KDIR) M=$(shell pwd) modules
clean:
	ake -C $(KDIR) M=$(shell pwd) clean
