cmake_minimum_required(VERSION 3.13)

project(LocusBeaconUtility
    DESCRIPTION "A cross-platform dekstop application for Locus"
    LANGUAGES CXX C)

# if(CMAKE_SYSTEM_NAME MATCHES "Linux")
include(FindPkgConfig)
if(PKG_CONFIG_FOUND)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(GTKMM REQUIRED IMPORTED_TARGET gtkmm-3.0 glibmm-2.4)
    message(">>>> pkg-config was found! >>>>" )
else()
    message(FATAL_ERROR " >>>> pkg-config was NOT found! >>>>" )
endif()
# endif()

if(WIN32)
    set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build_win")
elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(PROJECT_BINARY_DIR "${PROJECT_SOURCE_DIR}/build_linux")
endif()

if(WIN32)
    # Including DLL libraries for Windows
    link_directories("${CMAKE_CURRENT_SOURCE_DIR}/WindowsDLL")

    set(DLL_PATH ${CMAKE_CURRENT_SOURCE_DIR}/Platforms/Windows)

    message(" <<< DLL path is: ${DLL_PATH}")

    add_library(dll_win SHARED IMPORTED)
    set_target_properties(dll_win PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/lib
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_BINARY_DIR}/bin
        IMPORTED_LOCATION 
        ${DLL_PATH}/libatkmm-1.6-1.dll
        ${DLL_PATH}/libglib-2.0-0.dll
        ${DLL_PATH}/libgiomm-2.4-1.dll
        ${DLL_PATH}/libgcc_s_seh-1.dll
        # ${DLL_PATH}/api-ms-win-core-kernel32-legacy-l1-1-2.dll
    )
endif()

message("NEW BINARY DIR: ${PROJECT_BINARY_DIR}")

if (CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(FLAGS_COMMON
        "-pthread"
    )
endif()

set(FLAGS_COMMON
        # "-pthread"
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
    # install(FILES
    #     "${CMAKE_SOURCE_DIR}/x86_64-w64-mingw32.cmake"
    #     DESTINATION ${CMAKE_SOURCE_DIR}/x86_64-w64-mingw32.cmake)

elseif(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(CMAKE_EXECUTABLE_SUFFIX ".elf")
endif()

include_directories("${CMAKE_CURRENT_LIST_DIR}/Main")
include_directories("${CMAKE_CURRENT_LIST_DIR}/Sources")

set(SOURCE_FILES 
    Main/main.cpp
    Sources/MainWindow.cpp
)

# Add <Sources> subdir
add_subdirectory(Sources)

# Add <Cusb> subdir
add_subdirectory(Sources/Core/CUsb)

# Add <DroneDevice> subdir
add_subdirectory(Sources/DroneDevice)

add_executable(${CMAKE_PROJECT_NAME} 
    ${SOURCE_FILES}
    $<TARGET_OBJECTS:CustomWidgetsObj>
    $<TARGET_OBJECTS:GlobalHandlerEventsObj>
    $<TARGET_OBJECTS:SystemSerialObj>
)

target_link_libraries(${CMAKE_PROJECT_NAME} usb-1.0 DroneDevice)

if (WIN32)
    target_link_libraries(${CMAKE_PROJECT_NAME} dll_win)
endif()

if (GTKMM_FOUND)
    message(" >>>> Gtkmm was found! >>>>")
    target_link_libraries(${CMAKE_PROJECT_NAME} ${GTKMM_LIBRARIES})
    target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_INCLUDE_DIRS})
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC ${GTKMM_CFLAGS_OTHER})
else()
    message(" >>>> Gtkmm was NOT found! >>>>")
endif()