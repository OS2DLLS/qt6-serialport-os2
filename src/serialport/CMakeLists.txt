#####################################################################
## SerialPort Module:
#####################################################################
qt_find_package(Libudev PROVIDED_TARGETS PkgConfig::Libudev MODULE_NAME global QMAKE_LIB libudev)

qt_internal_add_module(SerialPort
    OS2_SHORT_NAME SP
    SOURCES
        qserialport.cpp qserialport.h qserialport_p.h
        qserialportglobal.h
        qserialportinfo.cpp qserialportinfo.h qserialportinfo_p.h

    INCLUDE_DIRECTORIES
        ${CMAKE_CURRENT_SOURCE_DIR}
    LIBRARIES
        Qt::CorePrivate
    PUBLIC_LIBRARIES
        Qt::Core
    PRIVATE_MODULE_INTERFACE
        Qt::CorePrivate
)

if(OS2)
    if(NOT OS2EMX_PREFIX OR NOT OS2EMX_TARGET)
        message(FATAL_ERROR "OS2EMX_PREFIX and OS2EMX_TARGET must be set for an OS/2 build")
    endif()
    string(REGEX MATCH "^[0-9]+" _os2emx_gcc_major "${CMAKE_CXX_COMPILER_VERSION}")
    if(NOT _os2emx_gcc_major)
        message(FATAL_ERROR "Unable to determine the OS/2 GCC include version")
    endif()
    set(_os2emx_cxx_include_dirs
        "${OS2EMX_PREFIX}/${OS2EMX_TARGET}/include/c++/${_os2emx_gcc_major}"
        "${OS2EMX_PREFIX}/${OS2EMX_TARGET}/include/c++/${_os2emx_gcc_major}/${OS2EMX_TARGET}"
        "${OS2EMX_PREFIX}/${OS2EMX_TARGET}/include/c++/${_os2emx_gcc_major}/backward"
        "${OS2EMX_PREFIX}/lib/gcc/${OS2EMX_TARGET}/${_os2emx_gcc_major}/include"
        "${OS2EMX_PREFIX}/lib/gcc/${OS2EMX_TARGET}/${_os2emx_gcc_major}/include-fixed"
    )
    foreach(include_dir IN LISTS _os2emx_cxx_include_dirs)
        if(NOT IS_DIRECTORY "${include_dir}")
            message(FATAL_ERROR "Required OS/2 C++ include directory is missing: ${include_dir}")
        endif()
    endforeach()
    target_include_directories(SerialPort PRIVATE ${_os2emx_cxx_include_dirs})

    # The OS/2 C front end overflows while mangling system-call names; this shim
    # keeps its C linkage from qserialport_os2.h but is compiled as C++.
    set_source_files_properties(qserialport_os2.c PROPERTIES LANGUAGE CXX)
    # The OS/2 loader requires a short on-disk DLL name.
    set_target_properties(SerialPort PROPERTIES OUTPUT_NAME "Qt6SP")

    if(DEFINED OS2EMX_PREFIX AND DEFINED OS2EMX_TARGET)
        find_program(EMXIMP_EXECUTABLE NAMES emximp
            HINTS "${OS2EMX_PREFIX}/${OS2EMX_TARGET}/bin")
    else()
        find_program(EMXIMP_EXECUTABLE NAMES emximp)
    endif()
    if(NOT EMXIMP_EXECUTABLE)
        message(FATAL_ERROR "emximp is required to create the OS/2 OMF import library")
    endif()
    add_custom_command(TARGET SerialPort POST_BUILD
        COMMAND "${EMXIMP_EXECUTABLE}" -p 128 -o
            "$<TARGET_LINKER_FILE:SerialPort>" "$<TARGET_FILE:SerialPort>"
        COMMENT "Generating OS/2 OMF import library"
        VERBATIM)
endif()

## Scopes:
#####################################################################

qt_internal_extend_target(SerialPort CONDITION config_ntddmodm
    DEFINES
        QT_NO_REDEFINE_GUID_DEVINTERFACE_MODEM
)

qt_internal_extend_target(SerialPort CONDITION QT_FEATURE_libudev AND UNIX
   DEFINES
       LINK_LIBUDEV
   LIBRARIES
       PkgConfig::Libudev
)

qt_internal_extend_target(SerialPort CONDITION WIN32
    SOURCES
        qserialport_win.cpp
        qserialportinfo_win.cpp
        qtntdll_p.h
    LIBRARIES
        advapi32
        setupapi
)

qt_internal_extend_target(SerialPort CONDITION UNIX
    SOURCES
        qserialport_unix.cpp
)

qt_internal_extend_target(SerialPort CONDITION OS2
    SOURCES
        qserialport_unix.cpp
        qserialportinfo_os2.cpp
        qserialport_os2.c
    COMPILE_OPTIONS
        -fno-unit-at-a-time
)

qt_internal_extend_target(SerialPort CONDITION MACOS
    SOURCES
        qserialportinfo_osx.cpp
    LIBRARIES
        ${FWCoreFoundation}
        ${FWIOKit}
)

qt_internal_extend_target(SerialPort CONDITION FREEBSD
    SOURCES
        qserialportinfo_freebsd.cpp
)

qt_internal_extend_target(SerialPort CONDITION UNIX AND NOT FREEBSD AND NOT MACOS
    SOURCES
        qserialportinfo_unix.cpp
)

qt_internal_add_docs(SerialPort
    doc/qtserialport.qdocconf
)

