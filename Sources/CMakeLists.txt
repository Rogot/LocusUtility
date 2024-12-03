message("Sources current CMAKE dir: ${CMAKE_CURRENT_LIST_DIR}")

include_directories("${CMAKE_CURRENT_LIST_DIR}/CustomWidgets")

set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/CustomWidgets" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/GlobalHandlerEvents" ${CMAKE_MODULE_PATH})
set(CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/Core/CUsb" ${CMAKE_MODULE_PATH})

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
target_link_libraries(GlobalHandlerEventsObj PUBLIC ${GTKMM_LIBRARIES})

# # Make library <CUsb>
# file(GLOB_RECURSE SOURCE_LIST_CUSB "Core/CUsb/CUsb.cpp")
# message("${SOURCE_LIST_CUSB} - SOURCE LIST (+ CUsb)")
# add_library(CUsbObj OBJECT ${SOURCE_LIST_CUSB})
# target_include_directories(CUsbObj PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Add <Cusb>
add_subdirectory(Core/CUsb)

# Add <DroneDevice> submodule
add_subdirectory(DroneDevice)