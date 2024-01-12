#----------------------------------------------------------------
# Generated CMake target import file.
#----------------------------------------------------------------

# Commands may need to know the format version.
set(CMAKE_IMPORT_FILE_VERSION 1)

# Import target "ical" for configuration ""
set_property(TARGET ical APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ical PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "-lpthread;/home/linuxdzm/alientek_workspace/ycoto/fsl-release-bsp/buildQt5.12.9/tmp/sysroots/imx6ull14x14evk/usr/lib/libicuuc.so;/home/linuxdzm/alientek_workspace/ycoto/fsl-release-bsp/buildQt5.12.9/tmp/sysroots/imx6ull14x14evk/usr/lib/libicui18n.so"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libical.so.2.0.0"
  IMPORTED_SONAME_NOCONFIG "libical.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS ical )
list(APPEND _IMPORT_CHECK_FILES_FOR_ical "/usr/lib/libical.so.2.0.0" )

# Import target "ical-static" for configuration ""
set_property(TARGET ical-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ical-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libical.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ical-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_ical-static "/usr/lib/libical.a" )

# Import target "ical_cxx" for configuration ""
set_property(TARGET ical_cxx APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ical_cxx PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "ical;-lpthread"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libical_cxx.so.2.0.0"
  IMPORTED_SONAME_NOCONFIG "libical_cxx.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS ical_cxx )
list(APPEND _IMPORT_CHECK_FILES_FOR_ical_cxx "/usr/lib/libical_cxx.so.2.0.0" )

# Import target "ical_cxx-static" for configuration ""
set_property(TARGET ical_cxx-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(ical_cxx-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libical_cxx.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS ical_cxx-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_ical_cxx-static "/usr/lib/libical_cxx.a" )

# Import target "icalss" for configuration ""
set_property(TARGET icalss APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(icalss PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "ical"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libicalss.so.2.0.0"
  IMPORTED_SONAME_NOCONFIG "libicalss.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalss )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalss "/usr/lib/libicalss.so.2.0.0" )

# Import target "icalss-static" for configuration ""
set_property(TARGET icalss-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(icalss-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libicalss.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalss-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalss-static "/usr/lib/libicalss.a" )

# Import target "icalss_cxx" for configuration ""
set_property(TARGET icalss_cxx APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(icalss_cxx PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "icalss;ical_cxx;-lpthread"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libicalss_cxx.so.2.0.0"
  IMPORTED_SONAME_NOCONFIG "libicalss_cxx.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalss_cxx )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalss_cxx "/usr/lib/libicalss_cxx.so.2.0.0" )

# Import target "icalss_cxx-static" for configuration ""
set_property(TARGET icalss_cxx-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(icalss_cxx-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "CXX"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libicalss_cxx.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalss_cxx-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalss_cxx-static "/usr/lib/libicalss_cxx.a" )

# Import target "icalvcal" for configuration ""
set_property(TARGET icalvcal APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(icalvcal PROPERTIES
  IMPORTED_LINK_INTERFACE_LIBRARIES_NOCONFIG "ical"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libicalvcal.so.2.0.0"
  IMPORTED_SONAME_NOCONFIG "libicalvcal.so.2"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalvcal )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalvcal "/usr/lib/libicalvcal.so.2.0.0" )

# Import target "icalvcal-static" for configuration ""
set_property(TARGET icalvcal-static APPEND PROPERTY IMPORTED_CONFIGURATIONS NOCONFIG)
set_target_properties(icalvcal-static PROPERTIES
  IMPORTED_LINK_INTERFACE_LANGUAGES_NOCONFIG "C"
  IMPORTED_LOCATION_NOCONFIG "/usr/lib/libicalvcal.a"
  )

list(APPEND _IMPORT_CHECK_TARGETS icalvcal-static )
list(APPEND _IMPORT_CHECK_FILES_FOR_icalvcal-static "/usr/lib/libicalvcal.a" )

# Commands beyond this point should not need to know the version.
set(CMAKE_IMPORT_FILE_VERSION)
