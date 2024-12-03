cmake_minimum_required(VERSION 3.13)
project(LocusBeaconUtiliy CXX)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    include(FindPkgConfig)
    if(PKG_CONFIG_FOUND)
        find_package(PkgConfig REQUIRED)
        pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0 glibmm-2.4)
    else()
        message(FATAL_ERROR " >>>> pkg-config was not found! >>>>" )
    endif()
endif()

if(WIN32)
    set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build_windows")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build_linux")
endif()

message("NEW BINARY DIR: ${PROJECT_BINARY_DIR}")

set(FLAGS_COMMON
        "-pthread"
        "-pedantic"
        "-Wall"
        "-Wcast-align"
        "-Wcast-qual"
        "-Wconversion"
        "-Wextra"
        "-Wshadow"
        "-Wold-style-cast"
        "-Woverloaded-virtual"
)

string(REPLACE ";" " " FLAGS_COMMON "${FLAGS_COMMON}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${FLAGS_CPU} ${FLAGS_COMMON}")
set(CMAKE_SHARED_LIBRARY_LINK_CXX_FLAGS "")
if(WIN32)
    set(CMAKE_EXECUTABLE_SUFFIX ".exe")
    set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
    set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
    set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

    # install config files
    install(FILES
        "${CMAKE_SOURCE_DIR}/x86_64-w64-mingw32.cmake"
        DESTINATION ${CMAKE_SOURCE_DIR}/x86_64-w64-mingw32.cmake)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_EXECUTABLE_SUFFIX ".elf")
endif()

include_directories("${CMAKE_CURRENT_LIST_DIR}/Main")
include_directories("${CMAKE_CURRENT_LIST_DIR}/Sources")

set(SOURCE_FILES 
    Main/main.cpp
    Sources/MainWindow.cpp
)

add_subdirectory(Sources)

add_executable(${CMAKE_PROJECT_NAME} ${SOURCE_FILES} 
    $<TARGET_OBJECTS:CustomWidgetsObj>
    $<TARGET_OBJECTS:GlobalHandlerEventsObj>
    # $<TARGET_OBJECTS:CUsbObj>
)

target_link_libraries(${CMAKE_PROJECT_NAME} usb-1.0)

if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    if (GTKMM_FOUND)
        message(" >>>> Gtkmm was found! >>>>")

        target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})
        target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_INCLUDE_DIRS})
        target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_CFLAGS_OTHER})
    endif()
endif()