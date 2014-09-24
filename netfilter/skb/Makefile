MAKE=make
WERROR_CFLAGS=
#KSRC = /usr/src/kernels/$(BUILD_KERNEL)
KSRC = /lib/modules/`uname -r`/build
obj-m := hello.o
hello:
	$(MAKE) -C $(KSRC) SUBDIRS=$(shell pwd) modules
clean:
	rm -rf *.ko *.o *.mod*
