host_build {
    QT_CPU_FEATURES.arm = neon
} else {
    QT_CPU_FEATURES.arm = neon
}
QT.global_private.enabled_features = alloca_h alloca dbus dbus-linked gui libudev network posix_fallocate reduce_exports sql system-zlib testlib widgets xml
QT.global_private.disabled_features = sse2 alloca_malloc_h android-style-assets avx2 private_tests gc_binaries reduce_relocations release_tools stack-protector-strong
PKG_CONFIG_EXECUTABLE = $$[QT_HOST_PREFIX/get]/usr/bin/pkg-config
QMAKE_LIBS_DBUS = $$[QT_SYSROOT]/usr/lib/libdbus-1.so
QMAKE_INCDIR_DBUS = $$[QT_SYSROOT]/usr/include/dbus-1.0 $$[QT_SYSROOT]/usr/lib/dbus-1.0/include
QMAKE_LIBS_LIBUDEV = $$[QT_SYSROOT]/usr/lib/libudev.so
QT_COORD_TYPE = double
QMAKE_LIBS_ZLIB = $$[QT_SYSROOT]/usr/lib/libz.so
CONFIG -= precompile_header
CONFIG += compile_examples enable_new_dtags largefile neon silent
QT_BUILD_PARTS += examples libs tests tools
QT_HOST_CFLAGS_DBUS += -I$$[QT_SYSROOT]/usr/include/dbus-1.0 -I$$[QT_SYSROOT]/usr/lib/dbus-1.0/include

