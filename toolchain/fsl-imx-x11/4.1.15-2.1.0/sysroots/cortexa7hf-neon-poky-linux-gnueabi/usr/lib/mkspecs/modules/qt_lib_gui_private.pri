QT.gui_private.VERSION = 5.12.9
QT.gui_private.name = QtGui
QT.gui_private.module =
QT.gui_private.libs = $$QT_MODULE_LIB_BASE
QT.gui_private.includes = $$QT_MODULE_INCLUDE_BASE/QtGui/5.12.9 $$QT_MODULE_INCLUDE_BASE/QtGui/5.12.9/QtGui
QT.gui_private.frameworks =
QT.gui_private.depends = core_private gui
QT.gui_private.uses =
QT.gui_private.module_config = v2 internal_module
QT.gui_private.enabled_features = xkbcommon xcb accessibility-atspi-bridge drm_atomic egl egl_x11 evdev freetype gif harfbuzz ico jpeg linuxfb multiprocess png system-freetype system-jpeg system-png system-xcb tslib tuiotouch vnc xcb-egl-plugin xlib xcb-xlib xkb
QT.gui_private.disabled_features = dxguid angle_d3d11_qdtd direct2d direct2d1_1 dxgi direct3d11 direct3d11_1 direct3d9 directfb dxgi1_2 eglfs eglfs_brcm eglfs_egldevice eglfs_gbm eglfs_mali eglfs_openwfd eglfs_rcar eglfs_viv eglfs_viv_wl eglfs_vsp2 eglfs_x11 fontconfig integrityfb integrityhid kms libinput libinput-axis-api mirclient mtdev system-harfbuzz texture_format_astc_experimental vsp2 xcb-glx-plugin xcb-native-painting xcb-sm xcb-xinput
QMAKE_LIBS_XKBCOMMON = $$[QT_SYSROOT]/usr/lib/libxkbcommon.so
QMAKE_LIBS_XCB = $$[QT_SYSROOT]/usr/lib/libxcb.so
QMAKE_LIBS_ATSPI = $$[QT_SYSROOT]/usr/lib/libatspi.so $$[QT_SYSROOT]/usr/lib/libdbus-1.so $$[QT_SYSROOT]/usr/lib/libglib-2.0.so
QMAKE_INCDIR_ATSPI = $$[QT_SYSROOT]/usr/include/at-spi-2.0 $$[QT_SYSROOT]/usr/include/dbus-1.0 $$[QT_SYSROOT]/usr/lib/dbus-1.0/include $$[QT_SYSROOT]/usr/include/glib-2.0 $$[QT_SYSROOT]/usr/lib/glib-2.0/include
QMAKE_LIBS_OPENGL_ES2 = $$[QT_SYSROOT]/usr/lib/libGLESv2.so
QMAKE_INCDIR_OPENGL_ES2 = 
QMAKE_LIBS_DRM = $$[QT_SYSROOT]/usr/lib/libdrm.so
QMAKE_INCDIR_DRM = $$[QT_SYSROOT]/usr/include/libdrm
QMAKE_LIBS_EGL = $$[QT_SYSROOT]/usr/lib/libEGL.so
QMAKE_INCDIR_EGL = $$[QT_SYSROOT]/usr/include/libdrm
QMAKE_LIBS_XLIB = $$[QT_SYSROOT]/usr/lib/libXext.so $$[QT_SYSROOT]/usr/lib/libX11.so $$[QT_SYSROOT]/usr/lib/libm.so
QMAKE_DEPENDS_FREETYPE_CC = ZLIB
QMAKE_DEPENDS_FREETYPE_LD = ZLIB
QMAKE_LIBS_FREETYPE = $$[QT_SYSROOT]/usr/lib/libfreetype.so
QMAKE_INCDIR_FREETYPE = $$[QT_SYSROOT]/usr/include/freetype2
QMAKE_LIBS_LIBJPEG = $$[QT_SYSROOT]/usr/lib/libjpeg.so
QMAKE_DEPENDS_LIBPNG_CC = ZLIB
QMAKE_DEPENDS_LIBPNG_LD = ZLIB
QMAKE_LIBS_LIBPNG = $$[QT_SYSROOT]/usr/lib/libpng16.so
QMAKE_INCDIR_LIBPNG = $$[QT_SYSROOT]/usr/include/libpng16
QMAKE_DEPENDS_XCB_ICCCM_CC = XCB
QMAKE_DEPENDS_XCB_ICCCM_LD = XCB
QMAKE_LIBS_XCB_ICCCM = $$[QT_SYSROOT]/usr/lib/libxcb-icccm.so
QMAKE_DEPENDS_XCB_SHM_CC = XCB
QMAKE_DEPENDS_XCB_SHM_LD = XCB
QMAKE_LIBS_XCB_SHM = $$[QT_SYSROOT]/usr/lib/libxcb-shm.so
QMAKE_DEPENDS_XCB_IMAGE_CC = XCB_SHM XCB
QMAKE_DEPENDS_XCB_IMAGE_LD = XCB_SHM XCB
QMAKE_LIBS_XCB_IMAGE = $$[QT_SYSROOT]/usr/lib/libxcb-image.so
QMAKE_DEPENDS_XCB_KEYSYMS_CC = XCB
QMAKE_DEPENDS_XCB_KEYSYMS_LD = XCB
QMAKE_LIBS_XCB_KEYSYMS = $$[QT_SYSROOT]/usr/lib/libxcb-keysyms.so
QMAKE_DEPENDS_XCB_RANDR_CC = XCB
QMAKE_DEPENDS_XCB_RANDR_LD = XCB
QMAKE_LIBS_XCB_RANDR = $$[QT_SYSROOT]/usr/lib/libxcb-randr.so $$[QT_SYSROOT]/usr/lib/libxcb-render.so
QMAKE_DEPENDS_XCB_RENDER_CC = XCB
QMAKE_DEPENDS_XCB_RENDER_LD = XCB
QMAKE_LIBS_XCB_RENDER = $$[QT_SYSROOT]/usr/lib/libxcb-render.so
QMAKE_DEPENDS_XCB_RENDERUTIL_CC = XCB XCB_RENDER
QMAKE_DEPENDS_XCB_RENDERUTIL_LD = XCB XCB_RENDER
QMAKE_LIBS_XCB_RENDERUTIL = $$[QT_SYSROOT]/usr/lib/libxcb-render-util.so
QMAKE_DEPENDS_XCB_SHAPE_CC = XCB
QMAKE_DEPENDS_XCB_SHAPE_LD = XCB
QMAKE_LIBS_XCB_SHAPE = $$[QT_SYSROOT]/usr/lib/libxcb-shape.so
QMAKE_DEPENDS_XCB_SYNC_CC = XCB
QMAKE_DEPENDS_XCB_SYNC_LD = XCB
QMAKE_LIBS_XCB_SYNC = $$[QT_SYSROOT]/usr/lib/libxcb-sync.so
QMAKE_DEPENDS_XCB_XFIXES_CC = XCB
QMAKE_DEPENDS_XCB_XFIXES_LD = XCB
QMAKE_LIBS_XCB_XFIXES = $$[QT_SYSROOT]/usr/lib/libxcb-xfixes.so $$[QT_SYSROOT]/usr/lib/libxcb-render.so $$[QT_SYSROOT]/usr/lib/libxcb-shape.so
QMAKE_DEPENDS_XCB_XINERAMA_CC = XCB
QMAKE_DEPENDS_XCB_XINERAMA_LD = XCB
QMAKE_LIBS_XCB_XINERAMA = $$[QT_SYSROOT]/usr/lib/libxcb-xinerama.so
QMAKE_LIBS_TSLIB = $$[QT_SYSROOT]/usr/lib/libts.so
QMAKE_DEPENDS_XCB_XLIB_CC = XCB XLIB
QMAKE_DEPENDS_XCB_XLIB_LD = XCB XLIB
QMAKE_LIBS_XCB_XLIB = $$[QT_SYSROOT]/usr/lib/libX11-xcb.so
QMAKE_DEPENDS_XCB_XKB_CC = XCB
QMAKE_DEPENDS_XCB_XKB_LD = XCB
QMAKE_LIBS_XCB_XKB = $$[QT_SYSROOT]/usr/lib/libxcb-xkb.so
QMAKE_LIBS_XKBCOMMON_X11 = $$[QT_SYSROOT]/usr/lib/libxkbcommon-x11.so $$[QT_SYSROOT]/usr/lib/libxkbcommon.so