cmake_minimum_required(VERSION 3.13)
project(UsbCommunication CXX C)

message("Sources current CMAKE dir (UsbCommunication): ${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

set(INCLUDE_CUSB_DIR "${CMAKE_CURRENT_SOURCE_DIR}/CUsb")

set(INCLUDE_DRONEDEVICE_DIR "${CMAKE_SOURCE_DIR}/Sources/DroneDevice/Include")

# set(INCLUDE_DRONEDEVICE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../../DroneDevice/Include")

message("DRONEDEVICE PATH: ${INCLUDE_DRONEDEVICE_DIR}")

# Add <Cusb> subdir
add_subdirectory(CUsb)

# Make library <UsbCommunication>
set(SOURCE_LIST_USBCOMMUNICATION
    "UsbCommunication.cpp"
    "UsbCommunication.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/SystemSerial/SystemSerial.cpp"
)

list(APPEND EXTRA_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}/SystemSerial"
)

message("${SOURCE_LIST_USBCOMMUNICATION} - SOURCE LIST (+ UsbCommunication)")
add_library(${PROJECT_NAME} 
    STATIC
        ${SOURCE_LIST_USBCOMMUNICATION}
        ${EXTRA_INCLUDES}
)

target_link_libraries(${PROJECT_NAME} PUBLIC DroneDevice)

target_include_directories(${PROJECT_NAME} 
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${INCLUDE_DRONEDEVICE_DIR}
)

target_include_directories(${PROJECT_NAME} 
    PRIVATE
        ${INCLUDE_CUSB_DIR}
        ${EXTRA_INCLUDES}
        # ${INCLUDE_DRONEDEVICE_DIR}
)

target_link_libraries(${PROJECT_NAME} PRIVATE usb-1.0)
# target_link_libraries(${CMAKE_PROJECT_NAME} UsbCommunicationObj)