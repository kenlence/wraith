QT.bluetooth_private.VERSION = 5.12.9
QT.bluetooth_private.name = QtBluetooth
QT.bluetooth_private.module =
QT.bluetooth_private.libs = $$QT_MODULE_LIB_BASE
QT.bluetooth_private.includes = $$QT_MODULE_INCLUDE_BASE/QtBluetooth/5.12.9 $$QT_MODULE_INCLUDE_BASE/QtBluetooth/5.12.9/QtBluetooth
QT.bluetooth_private.frameworks =
QT.bluetooth_private.depends = core_private bluetooth dbus network
QT.bluetooth_private.uses =
QT.bluetooth_private.module_config = v2 internal_module
QT.bluetooth_private.enabled_features = bluez_le linux_crypto_api
QT.bluetooth_private.disabled_features = winrt_bt
QMAKE_LIBS_BLUEZ = $$[QT_SYSROOT]/usr/lib/libbluetooth.so
