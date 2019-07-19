add_contract(vankia.trans vankia.trans ${CMAKE_CURRENT_SOURCE_DIR}/vankia.trans.cpp)
target_include_directories(vankia.trans.wasm
   PUBLIC
   ${CMAKE_CURRENT_SOURCE_DIR}
   ${CMAKE_CURRENT_SOURCE_DIR}/../eosio.token/include)

set_target_properties(vankia.trans.wasm
   PROPERTIES
   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}")
