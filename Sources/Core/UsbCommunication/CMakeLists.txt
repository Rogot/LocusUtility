cmake_minimum_required(VERSION 3.13)
project(UsbCommunication CXX C)

message("Sources current CMAKE dir (UsbCommunication): ${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

# Add <Cusb> subdir
add_subdirectory(CUsb)

# Make library <UsbCommunication>

set(SOURCE_LIST_USBCOMMUNICATION
    "UsbCommunication.cpp"
    "SystemSerial/SystemSerial.cpp"
)

message("${SOURCE_LIST_USBCOMMUNICATION} - SOURCE LIST (+ UsbCommunication)")
add_library(UsbCommunicationObj OBJECT "${SOURCE_LIST_USBCOMMUNICATION}")

list(APPEND EXTRA_INCLUDES
    "${CMAKE_CURRENT_SOURCE_DIR}"
    "${CMAKE_CURRENT_SOURCE_DIR}/SystemSerial")

target_include_directories(UsbCommunicationObj PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(UsbCommunicationObj PUBLIC usb-1.0 DroneDevice)

target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${EXTRA_INCLUDES})
target_link_libraries(${CMAKE_PROJECT_NAME} UsbCommunicationObj)