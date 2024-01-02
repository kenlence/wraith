KERNELDIR := ../../kernel/
CURRENT_PATH := $(shell pwd)

MOD_NAME := binder
obj-m := $(MOD_NAME).o

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	cp $(MOD_NAME).ko ../../out/modules/

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean