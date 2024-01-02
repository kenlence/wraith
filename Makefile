V=@

.PHONY: all clean kernel rootfs

all:kernel rootfs
	$V tar -zcf system.tar.gz out/

prepare:
	mkdir -p out/

kernel:prepare
	$V cd kernel && make imx_v7_wraith_defconfig && make all
	$V cp kernel/arch/arm/boot/zImage out/
	$V cp kernel/arch/arm/boot/dts/imx6ull-wraith.dtb out/

dts:prepare
	$V cd kernel && make dtbs
	$V cp kernel/arch/arm/boot/dts/imx6ull-wraith.dtb out/

rootfs:prepare
	$V mkdir -p out/rootfs/
	$V cd rootfs && make wraith_defconfig && make && make install CONFIG_PREFIX=../out/rootfs

	$V mkdir -p out/rootfs/lib
	$V cp toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/*so* out/rootfs/lib/ -d
	$V cp toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/lib/*.a out/rootfs/lib/ -d
	$V cp toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib/*so* out/rootfs/lib/ -d
	$V cp toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/lib/*.a out/rootfs/lib/ -d

	$V mkdir -p out/rootfs/usr/lib/
	$V cp toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc//usr/lib/*so* out/rootfs/usr/lib/ -d
	$V cp toolchain/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/arm-linux-gnueabihf/libc/usr/lib/*.a out/rootfs/usr/lib/ -d

	$V mkdir -p out/rootfs/dev/
	$V mkdir -p out/rootfs/proc/
	$V mkdir -p out/rootfs/mnt/
	$V mkdir -p out/rootfs/sys/
	$V mkdir -p out/rootfs/tmp/
	$V mkdir -p out/rootfs/root/
	$V mkdir -p out/rootfs/opt/
	$V mkdir -p out/rootfs/etc/
	$V mkdir -p out/rootfs/system/

# still can't work, I guess it works but didn't print to serial 0
qemu:
	qemu-system-arm \
		-M mcimx6ul-evk \
		-m 1024M  \
		-nographic \
		-kernel out/zImage \
		-dtb out/imx6ull-wraith.dtb \
		-append "console=ttyS0" \
		-serial mon:stdio

clean:
	$V rm out/*
	$V cd kernel && make clean




