MAKE=make
BUILD_KERNEL=3.14.8-200.fc20.i686
#KSRC = /usr/src/kernels/$(BUILD_KERNEL)
KSRC = /lib/modules/`uname -r`/build
obj-m := genl.o
hello:
	$(MAKE) -C $(KSRC) SUBDIRS=$(shell pwd) modules
clean:
	rm -rf *.ko *.o *.mod* modules.* Module.*
