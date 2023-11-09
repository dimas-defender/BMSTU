obj-m += my_workqueue.o

KDIR = /lib/modules/$(shell uname -r)/build

all default: modules
install: modules_install

modules modules_install help clean:
	make -C $(KDIR) M=$(shell pwd) $@
