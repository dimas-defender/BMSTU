obj-m += myfs.o

KVER = $(shell uname -r)
KDIR = /lib/modules/$(KVER)/build
PWD = $(shell pwd)

all default: modules
install: modules_install

modules modules_install help clean:
	make -C $(KDIR) M=$(PWD) $@
