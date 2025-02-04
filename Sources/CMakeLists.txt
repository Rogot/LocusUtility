message("Sources current CMAKE dir: ${CMAKE_CURRENT_LIST_DIR}")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CustomWidgets" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/GlobalHandlerEvents" ${CMAKE_MODULE_PATH})

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
target_include_directories(GlobalHandlerEventsObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add <DroneDevice> subdir
add_subdirectory(DroneDevice)
# Add <UsbCommunication> subdir
add_subdirectory(Core/UsbCommunication)