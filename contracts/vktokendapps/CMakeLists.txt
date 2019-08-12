add_contract(vktokendapps vktokendapps ${CMAKE_CURRENT_SOURCE_DIR}/vktokendapps.cpp)
target_include_directories(vktokendapps
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR})

set_target_properties(vktokendapps
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
