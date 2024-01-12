
if (CMAKE_VERSION VERSION_LESS 3.1.0)
    message(FATAL_ERROR "Qt 5 ServiceFramework module requires at least CMake version 3.1.0")
endif()

if(NOT DEFINED OE_QMAKE_PATH_EXTERNAL_HOST_BINS)
    message(WARNING Skipping because OE_QMAKE_PATH_EXTERNAL_HOST_BINS is not defined)
    return()
endif()

get_filename_component(_IMPORT_PREFIX "${CMAKE_CURRENT_LIST_FILE}" PATH)
# Use original install prefix when loaded through a
# cross-prefix symbolic link such as /lib -> /usr/lib.
get_filename_component(_realCurr "${_IMPORT_PREFIX}" REALPATH)
get_filename_component(_realOrig "/usr/lib/cmake/Qt5ServiceFramework" REALPATH)
if(_realCurr STREQUAL _realOrig)
    get_filename_component(_qt5ServiceFramework_install_prefix "/usr/lib/../" ABSOLUTE)
else()
    get_filename_component(_qt5ServiceFramework_install_prefix "${CMAKE_CURRENT_LIST_DIR}/../../../" ABSOLUTE)
endif()
unset(_realOrig)
unset(_realCurr)
unset(_IMPORT_PREFIX)

# For backwards compatibility only. Use Qt5ServiceFramework_VERSION instead.
set(Qt5ServiceFramework_VERSION_STRING 5.4.0)

set(Qt5ServiceFramework_LIBRARIES Qt5::ServiceFramework)

macro(_qt5_ServiceFramework_check_file_exists file)
    if(NOT EXISTS "${file}" )
        message(FATAL_ERROR "The imported target \"Qt5::ServiceFramework\" references the file
   \"${file}\"
but this file does not exist.  Possible reasons include:
* The file was deleted, renamed, or moved to another location.
* An install or uninstall procedure did not complete successfully.
* The installation package was faulty and contained
   \"${CMAKE_CURRENT_LIST_FILE}\"
but not all the files it references.
")
    endif()
endmacro()

macro(_populate_ServiceFramework_target_properties Configuration LIB_LOCATION IMPLIB_LOCATION)
    set_property(TARGET Qt5::ServiceFramework APPEND PROPERTY IMPORTED_CONFIGURATIONS ${Configuration})

    set(imported_location "${_qt5ServiceFramework_install_prefix}/lib/${LIB_LOCATION}")
    _qt5_ServiceFramework_check_file_exists(${imported_location})
    set_target_properties(Qt5::ServiceFramework PROPERTIES
        "INTERFACE_LINK_LIBRARIES" "${_Qt5ServiceFramework_LIB_DEPENDENCIES}"
        "IMPORTED_LOCATION_${Configuration}" ${imported_location}
        "IMPORTED_SONAME_${Configuration}" "libQt5ServiceFramework.so.5"
        # For backward compatibility with CMake < 2.8.12
        "IMPORTED_LINK_INTERFACE_LIBRARIES_${Configuration}" "${_Qt5ServiceFramework_LIB_DEPENDENCIES}"
    )

endmacro()

if (NOT TARGET Qt5::ServiceFramework)

    set(_Qt5ServiceFramework_OWN_INCLUDE_DIRS "${_qt5ServiceFramework_install_prefix}/include/" "${_qt5ServiceFramework_install_prefix}/include/QtServiceFramework")
    set(Qt5ServiceFramework_PRIVATE_INCLUDE_DIRS
        "${_qt5ServiceFramework_install_prefix}/include/QtServiceFramework/5.4.0"
        "${_qt5ServiceFramework_install_prefix}/include/QtServiceFramework/5.4.0/QtServiceFramework"
    )

    foreach(_dir ${_Qt5ServiceFramework_OWN_INCLUDE_DIRS})
        _qt5_ServiceFramework_check_file_exists(${_dir})
    endforeach()

    # Only check existence of private includes if the Private component is
    # specified.
    list(FIND Qt5ServiceFramework_FIND_COMPONENTS Private _check_private)
    if (NOT _check_private STREQUAL -1)
        foreach(_dir ${Qt5ServiceFramework_PRIVATE_INCLUDE_DIRS})
            _qt5_ServiceFramework_check_file_exists(${_dir})
        endforeach()
    endif()

    set(Qt5ServiceFramework_INCLUDE_DIRS ${_Qt5ServiceFramework_OWN_INCLUDE_DIRS})

    set(Qt5ServiceFramework_DEFINITIONS -DQT_SERVICEFRAMEWORK_LIB)
    set(Qt5ServiceFramework_COMPILE_DEFINITIONS QT_SERVICEFRAMEWORK_LIB)
    set(_Qt5ServiceFramework_MODULE_DEPENDENCIES "Core")


    set(Qt5ServiceFramework_OWN_PRIVATE_INCLUDE_DIRS ${Qt5ServiceFramework_PRIVATE_INCLUDE_DIRS})

    set(_Qt5ServiceFramework_FIND_DEPENDENCIES_REQUIRED)
    if (Qt5ServiceFramework_FIND_REQUIRED)
        set(_Qt5ServiceFramework_FIND_DEPENDENCIES_REQUIRED REQUIRED)
    endif()
    set(_Qt5ServiceFramework_FIND_DEPENDENCIES_QUIET)
    if (Qt5ServiceFramework_FIND_QUIETLY)
        set(_Qt5ServiceFramework_DEPENDENCIES_FIND_QUIET QUIET)
    endif()
    set(_Qt5ServiceFramework_FIND_VERSION_EXACT)
    if (Qt5ServiceFramework_FIND_VERSION_EXACT)
        set(_Qt5ServiceFramework_FIND_VERSION_EXACT EXACT)
    endif()

    set(Qt5ServiceFramework_EXECUTABLE_COMPILE_FLAGS "")

    foreach(_module_dep ${_Qt5ServiceFramework_MODULE_DEPENDENCIES})
        if (NOT Qt5${_module_dep}_FOUND)
            find_package(Qt5${_module_dep}
                5.4.0 ${_Qt5ServiceFramework_FIND_VERSION_EXACT}
                ${_Qt5ServiceFramework_DEPENDENCIES_FIND_QUIET}
                ${_Qt5ServiceFramework_FIND_DEPENDENCIES_REQUIRED}
                PATHS "${CMAKE_CURRENT_LIST_DIR}/.." NO_DEFAULT_PATH
            )
        endif()

        if (NOT Qt5${_module_dep}_FOUND)
            set(Qt5ServiceFramework_FOUND False)
            return()
        endif()

        list(APPEND Qt5ServiceFramework_INCLUDE_DIRS "${Qt5${_module_dep}_INCLUDE_DIRS}")
        list(APPEND Qt5ServiceFramework_PRIVATE_INCLUDE_DIRS "${Qt5${_module_dep}_PRIVATE_INCLUDE_DIRS}")
        list(APPEND Qt5ServiceFramework_DEFINITIONS ${Qt5${_module_dep}_DEFINITIONS})
        list(APPEND Qt5ServiceFramework_COMPILE_DEFINITIONS ${Qt5${_module_dep}_COMPILE_DEFINITIONS})
        list(APPEND Qt5ServiceFramework_EXECUTABLE_COMPILE_FLAGS ${Qt5${_module_dep}_EXECUTABLE_COMPILE_FLAGS})
    endforeach()
    list(REMOVE_DUPLICATES Qt5ServiceFramework_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES Qt5ServiceFramework_PRIVATE_INCLUDE_DIRS)
    list(REMOVE_DUPLICATES Qt5ServiceFramework_DEFINITIONS)
    list(REMOVE_DUPLICATES Qt5ServiceFramework_COMPILE_DEFINITIONS)
    list(REMOVE_DUPLICATES Qt5ServiceFramework_EXECUTABLE_COMPILE_FLAGS)

    set(_Qt5ServiceFramework_LIB_DEPENDENCIES "Qt5::Core")


    add_library(Qt5::ServiceFramework SHARED IMPORTED)

    set_property(TARGET Qt5::ServiceFramework PROPERTY
      INTERFACE_INCLUDE_DIRECTORIES ${_Qt5ServiceFramework_OWN_INCLUDE_DIRS})
    set_property(TARGET Qt5::ServiceFramework PROPERTY
      INTERFACE_COMPILE_DEFINITIONS QT_SERVICEFRAMEWORK_LIB)

    set_property(TARGET Qt5::ServiceFramework PROPERTY INTERFACE_QT_ENABLED_FEATURES )
    set_property(TARGET Qt5::ServiceFramework PROPERTY INTERFACE_QT_DISABLED_FEATURES )

    set(_Qt5ServiceFramework_PRIVATE_DIRS_EXIST TRUE)
    foreach (_Qt5ServiceFramework_PRIVATE_DIR ${Qt5ServiceFramework_OWN_PRIVATE_INCLUDE_DIRS})
        if (NOT EXISTS ${_Qt5ServiceFramework_PRIVATE_DIR})
            set(_Qt5ServiceFramework_PRIVATE_DIRS_EXIST FALSE)
        endif()
    endforeach()

    if (_Qt5ServiceFramework_PRIVATE_DIRS_EXIST)
        add_library(Qt5::ServiceFrameworkPrivate INTERFACE IMPORTED)
        set_property(TARGET Qt5::ServiceFrameworkPrivate PROPERTY
            INTERFACE_INCLUDE_DIRECTORIES ${Qt5ServiceFramework_OWN_PRIVATE_INCLUDE_DIRS}
        )
        set(_Qt5ServiceFramework_PRIVATEDEPS)
        foreach(dep ${_Qt5ServiceFramework_LIB_DEPENDENCIES})
            if (TARGET ${dep}Private)
                list(APPEND _Qt5ServiceFramework_PRIVATEDEPS ${dep}Private)
            endif()
        endforeach()
        set_property(TARGET Qt5::ServiceFrameworkPrivate PROPERTY
            INTERFACE_LINK_LIBRARIES Qt5::ServiceFramework ${_Qt5ServiceFramework_PRIVATEDEPS}
        )
    endif()

    _populate_ServiceFramework_target_properties(RELEASE "libQt5ServiceFramework.so.5.4.0" "" )




    file(GLOB pluginTargets "${CMAKE_CURRENT_LIST_DIR}/Qt5ServiceFramework_*Plugin.cmake")

    macro(_populate_ServiceFramework_plugin_properties Plugin Configuration PLUGIN_LOCATION)
        set_property(TARGET Qt5::${Plugin} APPEND PROPERTY IMPORTED_CONFIGURATIONS ${Configuration})

        set(imported_location "${_qt5ServiceFramework_install_prefix}/lib/plugins/${PLUGIN_LOCATION}")
        _qt5_ServiceFramework_check_file_exists(${imported_location})
        set_target_properties(Qt5::${Plugin} PROPERTIES
            "IMPORTED_LOCATION_${Configuration}" ${imported_location}
        )
    endmacro()

    if (pluginTargets)
        foreach(pluginTarget ${pluginTargets})
            include(${pluginTarget})
        endforeach()
    endif()




_qt5_ServiceFramework_check_file_exists("${CMAKE_CURRENT_LIST_DIR}/Qt5ServiceFrameworkConfigVersion.cmake")

endif()