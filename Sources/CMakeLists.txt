message("Sources current CMAKE dir: ${CMAKE_CURRENT_LIST_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CustomWidgets" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/GlobalHandlerEvents" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/CUsb" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/SystemSerial" ${CMAKE_MODULE_PATH})

# Make library <CustomWidgets>
file(GLOB_RECURSE SOURCE_LIST_CUSTOM_WIDGETS "CustomWidgets/*.cpp")
message("${SOURCE_LIST_CUSTOM_WIDGETS} - SOURCE LIST (+ CustomWidgets)")
add_library(CustomWidgetsObj OBJECT ${SOURCE_LIST_CUSTOM_WIDGETS})
target_include_directories(CustomWidgetsObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${GTKMM_INCLUDE_DIRS})
target_link_libraries(CustomWidgetsObj PUBLIC ${GTKMM_LIBRARIES})

# Make library <GlobalHandlerEvents>
file(GLOB_RECURSE SOURCE_LIST_GLOBAL_HANDLER_EVENTS "Core/GlobalHandlerEvents/*.cpp")
message("${SOURCE_LIST_GLOBAL_HANDLER_EVENTS} - SOURCE LIST (+ GlobalHandlerEvents)")
add_library(GlobalHandlerEventsObj OBJECT ${SOURCE_LIST_GLOBAL_HANDLER_EVENTS})
target_include_directories(GlobalHandlerEventsObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${GTKMM_INCLUDE_DIRS})

# Make library <GlobalHandlerEvents>
file(GLOB_RECURSE SOURCE_LIST_SYSTEMSERIAL "Core/SystemSerial/*.cpp")
message("${SOURCE_LIST_SYSTEMSERIAL} - SOURCE LIST (+ SystemSerial)")
add_library(SystemSerialObj OBJECT ${SOURCE_LIST_SYSTEMSERIAL})
target_include_directories(SystemSerialObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR} ${GTKMM_INCLUDE_DIRS})
target_link_libraries(SystemSerialObj PUBLIC usb-1.0)
