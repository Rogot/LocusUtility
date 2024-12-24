message("Sources current CMAKE dir: ${CMAKE_CURRENT_SOURCE_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}" ${CMAKE_MODULE_PATH})

# Add <Cusb> subdir
add_subdirectory(CUsb)

# Make library <UsbCommunication>
file(GLOB_RECURSE SOURCE_LIST_USBCOMMUNICATION "UsbCommunication.cpp" "SystemSerial/*.cpp")
message("${SOURCE_LIST_USBCOMMUNICATION} - SOURCE LIST (+ UsbCommunication)")
add_library(UsbCommunicationObj OBJECT ${SOURCE_LIST_USBCOMMUNICATION})
target_include_directories(UsbCommunicationObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(UsbCommunicationObj PUBLIC usb-1.0 DroneDevice)


target_link_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${CMAKE_PROJECT_NAME} UsbCommunicationObj)