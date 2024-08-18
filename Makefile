CROSS_COMPILE := arm-linux-gnueabihf-

CC := $(CROSS_COMPILE)gcc
GPP := $(CROSS_COMPILE)g++

WRAITH_DIR := $(shell pwd)
KERNEL_DIR := $(WRAITH_DIR)/kernel
OUT_DIR := $(WRAITH_DIR)/out
ROOTFS_DIR := $(WRAITH_DIR)/rootfs
TOOLCHAIN_DIR := $(WRAITH_DIR)/toolchain
MODULES_DIR := $(WRAITH_DIR)/modules
SYSTEM_DIR := $(WRAITH_DIR)/system
TEST_DIR := $(WRAITH_DIR)/test

.PHONY: all clean kernel rootfs system

all:kernel rootfs modules
	tar -zcf system.tar.gz $(OUT_DIR)/

prepare:
	mkdir -p $(OUT_DIR)/

kernel:prepare
	cd $(KERNEL_DIR) && make imx_v7_wraith_defconfig && make all
	cp $(KERNEL_DIR)/arch/arm/boot/zImage $(OUT_DIR)/
	cp $(KERNEL_DIR)/arch/arm/boot/dts/imx6ull-wraith.dtb $(OUT_DIR)/

modules:prepare
	mkdir -p $(OUT_DIR)/modules/
	make -C $(MODULES_DIR)

dts:prepare
	cd $(KERNEL_DIR) && make dtbs
	cp $(KERNEL_DIR)/arch/arm/boot/dts/imx6ull-wraith.dtb $(OUT_DIR)/

system:prepare
	make -C $(SYSTEM_DIR)/

rootfs:prepare
	mkdir -p $(OUT_DIR)/rootfs/
	cd $(ROOTFS_DIR) && make wraith_defconfig && make && make install CONFIG_PREFIX=$(OUT_DIR)/rootfs

	mkdir -p $(OUT_DIR)/rootfs/lib
	mkdir -p $(OUT_DIR)/rootfs/lib/modules
	cp $(TOOLCHAIN_DIR)/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/*so* $(OUT_DIR)/rootfs/lib/ -d
	cp $(TOOLCHAIN_DIR)/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/*.a $(OUT_DIR)/rootfs/lib/ -d
	cp $(TOOLCHAIN_DIR)/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib/*so* $(OUT_DIR)/rootfs/lib/ -d
	cp $(TOOLCHAIN_DIR)/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib/*.a $(OUT_DIR)/rootfs/lib/ -d

	mkdir -p $(OUT_DIR)/rootfs/usr/lib/
	cp $(TOOLCHAIN_DIR)/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc//usr/lib/*so* $(OUT_DIR)/rootfs/usr/lib/ -d
	cp $(TOOLCHAIN_DIR)/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/lib/*.a $(OUT_DIR)/rootfs/usr/lib/ -d

	mkdir -p $(OUT_DIR)/rootfs/dev/
	mkdir -p $(OUT_DIR)/rootfs/proc/
	mkdir -p $(OUT_DIR)/rootfs/mnt/
	mkdir -p $(OUT_DIR)/rootfs/sys/
	mkdir -p $(OUT_DIR)/rootfs/tmp/
	mkdir -p $(OUT_DIR)/rootfs/root/
	mkdir -p $(OUT_DIR)/rootfs/opt/
	mkdir -p $(OUT_DIR)/rootfs/etc/
	mkdir -p $(OUT_DIR)/rootfs/system/
	mkdir -p $(OUT_DIR)/rootfs/system/bin/
	mkdir -p $(OUT_DIR)/rootfs/system/lib/
	mkdir -p $(OUT_DIR)/rootfs/var/
	mkdir -p $(OUT_DIR)/rootfs/var/log/

test:prepare
	mkdir -p $(OUT_DIR)/test
	make -C $(TEST_DIR)/

# still can't work, I guess it works but didn't print to serial 0
qemu:
	qemu-system-arm \
		-M mcimx6ul-evk \
		-m 1024M  \
		-nographic \
		-kernel $(OUT_DIR)/zImage \
		-dtb $(OUT_DIR)/imx6ull-wraith.dtb \
		-append "console=ttyS0" \
		-serial mon:stdio

clean:
	rm $(OUT_DIR)/*
	cd $(KERNEL_DIR) && make clean




