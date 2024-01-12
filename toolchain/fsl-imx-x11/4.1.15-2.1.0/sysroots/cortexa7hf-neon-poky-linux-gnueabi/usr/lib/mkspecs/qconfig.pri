!host_build {
    QMAKE_CFLAGS    += --sysroot=$$[QT_SYSROOT]
    QMAKE_CXXFLAGS  += --sysroot=$$[QT_SYSROOT]
    QMAKE_LFLAGS    += --sysroot=$$[QT_SYSROOT]
}
host_build {
    QT_ARCH = arm
    QT_BUILDABI = arm-little_endian-ilp32-eabi-hardfloat
    QT_TARGET_ARCH = arm
    QT_TARGET_BUILDABI = arm-little_endian-ilp32-eabi-hardfloat
} else {
    QT_ARCH = arm
    QT_BUILDABI = arm-little_endian-ilp32-eabi-hardfloat
}
QT.global.enabled_features = shared c++11 c++14 c99 c11 thread future concurrent pkg-config
QT.global.disabled_features = framework rpath appstore-compliant debug_and_release simulator_and_device build_all c++1z force_asserts separate_debug_info static
PKG_CONFIG_SYSROOT_DIR = $$[QT_SYSROOT]
PKG_CONFIG_LIBDIR = $$[QT_SYSROOT]/usr/lib/pkgconfig
CONFIG += shared release
QT_CONFIG += shared release c++11 c++14 concurrent dbus reduce_exports stl
QT_VERSION = 5.12.9
QT_MAJOR_VERSION = 5
QT_MINOR_VERSION = 12
QT_PATCH_VERSION = 9
QT_GCC_MAJOR_VERSION = 5
QT_GCC_MINOR_VERSION = 3
QT_GCC_PATCH_VERSION = 0
QT_EDITION = OpenSource
