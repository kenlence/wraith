KERNELDIR := ../../kernel/
CURRENT_PATH := $(shell pwd)

MOD_NAME := binder
obj-m := $(MOD_NAME).o

# 下面这个选项是在kernel下才发挥作用的，所以include的路径是相对kernel的路径
INCDIRS 		:= ../include/modules

INCLUDE			:= $(patsubst %, -I %, $(INCDIRS))

EXTRA_CFLAGS += $(INCLUDE)

build: kernel_modules

kernel_modules:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) modules
	cp $(MOD_NAME).ko ../../out/modules/

clean:
	$(MAKE) -C $(KERNELDIR) M=$(CURRENT_PATH) clean
