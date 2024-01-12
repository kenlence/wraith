#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "Qt5::WebKit" for configuration ""
set_property(TARGET Qt5::WebKit APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(Qt5::WebKit PROPERTIES
  IMPORTED_LINK_DEPENDENT_LIBRARIES_NOCONFIG "Qt5::Quick;Qt5::WebChannel;Qt5::Positioning"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libQt5WebKit.so.5.212.0"
  IMPORTED_SONAME_NOCONFIG "libQt5WebKit.so.5"
  )

list(APPEND _IMPORT_CHECK_TARGETS Qt5::WebKit )
list(APPEND _IMPORT_CHECK_FILES_FOR_Qt5::WebKit "/usr/lib/libQt5WebKit.so.5.212.0" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
