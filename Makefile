V=@

.PHONY: all clean kernel

all: kernel

prepare:
	mkdir -p out/

kernel:prepare
	$V cd kernel && make imx_v7_wraith_defconfig && make all
	$V cp kernel/arch/arm/boot/zImage out/
	$V cp kernel/arch/arm/boot/dts/imx6ull-wraith.dtb out/

dts:
	$V cd kernel && make dts
	$V cp kernel/arch/arm/boot/dts/imx6ull-wraith.dtb out/

clean:
	rm out/*
	cd kernel && make clean




