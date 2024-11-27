cmake_minimum_required(VERSION 3.13)
project(LocusBeaconUtiliy CXX)

include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
  message(FATAL_ERROR " >>>> pkg-config was not found! >>>>" )
endif()


find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0 glibmm-2.4)

set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build")

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
set(CMAKE_EXECUTABLE_SUFFIX ".elf")

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
    $<TARGET_OBJECTS:CUsbObj>
)

target_link_libraries(${CMAKE_PROJECT_NAME} usb-1.0)

if (GTKMM_FOUND)
    message(" >>>> Gtkmm was found! >>>>")

    target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_INCLUDE_DIRS})
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_CFLAGS_OTHER})
endif()