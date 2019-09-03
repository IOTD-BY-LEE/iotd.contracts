add_contract(vankia.hblf vankia.hblf ${CMAKE_CURRENT_SOURCE_DIR}/vankia.hblf.cpp)
target_include_directories(vankia.hblf
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../eosio.token/include
   ${CMAKE_CURRENT_SOURCE_DIR}/../eosio.system/include)

set_target_properties(vankia.hblf
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
