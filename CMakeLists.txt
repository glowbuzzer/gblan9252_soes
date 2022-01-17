cmake_minimum_required(VERSION 3.0)
project(gblan9252_soes C)

set(CMAKE_C_STANDARD 99)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake")

#FLAVOUR is the platform variant - currently only PI or LINUX
if (NOT DEFINED FLAVOUR)
    message(STATUS "GB: No -DFLAVOUR=\"blah\" was provided we will default to [LINUX]")
    set(FLAVOUR "LINUX")
elseif (FLAVOUR STREQUAL "PI")
    set(FLAVOUR "PI")
else ()
    message(STATUS "GB: We could not match the FLAVOUR you provided. We will default to [LINUX]")
    set(FLAVOUR "LINUX")
endif ()


add_compile_definitions(GB_APP_LINUX)


#Enables debug logging overall
add_compile_definitions(ENABLE_LOGGING=1)

#Sets the debug log level
#LOG_LVL_FATAL = 0
#LOG_LVL_ERROR = 1
#LOG_LVL_WARNING =  2
#LOG_LVL_INFO = 3
#LOG_LVL_DEBUG = 4
#LOG_LVL_TRACE = 5

add_compile_definitions(LOG_LEVEL=5)


#Enables user messages (standard messages that describe what is going on in GBEM output to console, log, syslog etc.)
add_compile_definitions(GBLAN9252_SOES_UM_EN=1)


#Enables general gbnetx debig logging (general logging statements in key places)
add_compile_definitions(GBLAN9252_SOES_GEN_LOG_EN=1)




find_package(BCM2835)

if (BCM2835_FOUND)
    message (STATUS "BCM2835 found")
else()
    message (STATUS "BCM2835 NOT found")
endif()

include_directories(${BCM2835_INCLUDE_DIRS} ${gbnetx_config_BINARY_DIR} soes inc soes/hal/raspberrypi-lan9252 soes/include/sys/gcc)

set(SOURCE_FILES
        src/main.c
        src/slave_objectlist.c
        soes/ecat_slv.c
        soes/esc.c
        soes/esc_coe.c
        soes/esc_eep.c
        soes/esc_eoe.c
        soes/esc_foe.c
        soes/hal/raspberrypi-lan9252/esc_hw.c
        )


add_executable(gblan9252_soes ${SOURCE_FILES})

add_subdirectory(libs/gblan9252_soes_config)
add_subdirectory(gclibs/logging)
add_subdirectory("gclibs/gberror")
add_subdirectory("gclibs/common-misc")
add_subdirectory("gclibs/linux-shm")
add_subdirectory("gclibs/pid")
add_subdirectory("gclibs/dpm")

target_link_libraries(gblan9252_soes  linux-shm pid dpm Logging gblan9252_soes_config common-misc m rt ${BCM2835_LIBRARIES})


MESSAGE(STATUS "GB: [TARGET_GBLAN9252_SOES_LINUX] is being used")
if (FLAVOUR STREQUAL "PI")
    MESSAGE(STATUS "GB: Loading [gblan9252_soes_defs_pi.cmake]")
    include(cmake/gblan9252_soes_defs_pi.cmake)
    if (EXISTS cmake/gblan9252_soes_custom_defs_pi.cmake)
        MESSAGE(STATUS "GB: Loading [gblan9252_soes_custom_defs_pi.cmake]")
        include(cmake/gblan9252_soes_custom_defs_pi.cmake)
    else ()
        MESSAGE(STATUS "GB: [gblan9252_soes_custom_def_pi.cmake] does NOT exist (no overrides for standard definitions will be used)")
    endif ()
else ()
    MESSAGE(STATUS "GB: Loading [gblan9252_soes_defs_linux.cmake]")
    include(cmake/gblan9252_soes_defs_linux.cmake)
    if (EXISTS cmake/gblan9252_soes_custom_defs_linux.cmake)
        MESSAGE(STATUS "GB: Loading [gblan9252_soes_custom_defs_linuxc.make]")
        include(cmake/gblan9252_soes_custom_defs_linux.cmake)
    else ()
        MESSAGE(STATUS "GB: gblan9252_soes_custom_defs_linux.cmake does NOT exist (no overrides for standard definitions will be used)")
    endif ()

endif ()
configure_file(cmake/gblan9252_soes_config_autogen.h.in libs/gblan9252_soes_config/gblan9252_soes_config_autogen.h)
