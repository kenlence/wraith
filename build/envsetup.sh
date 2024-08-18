#!/bin/bash

# TODO:这里还需要优化，现在只能手动写
WRAITH_PATH=/home/lucas/data/wraith
TOOLCHAIN_PATH=$WRAITH_PATH/toolchain

export WRAITH_PATH

# poky
export POKYSYSROOT=$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi
export PATH=$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/sbin:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/bin:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/sbin:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/../x86_64-pokysdk-linux/bin:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-uclibc:$TOOLCHAIN_PATH/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-musl:$PATH

# arm-linux-gnueabihf-
export PATH=$PATH:$TOOLCHAIN_PATH/gcc-linaro-4.9.4-2017.01-x86_64_arm-linux-gnueabihf/bin
