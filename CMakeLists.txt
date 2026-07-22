cmake_minimum_required(VERSION 3.15)
cmake_policy(SET CMP0042 NEW)
cmake_policy(SET CMP0091 NEW)

project(addon CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(${PROJECT_NAME} SHARED src/addon.cc ${CMAKE_JS_SRC})

set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB})

# Include node-addon-api headers.
execute_process(
  COMMAND node -p "require('node-addon-api').include_dir"
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  OUTPUT_VARIABLE NODE_ADDON_API_DIR
  OUTPUT_STRIP_TRAILING_WHITESPACE
)
string(REGEX REPLACE "[\r\n\"]" "" NODE_ADDON_API_DIR ${NODE_ADDON_API_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE ${NODE_ADDON_API_DIR})

target_compile_definitions(${PROJECT_NAME} PRIVATE NAPI_DISABLE_CPP_EXCEPTIONS)

if(MSVC)
  target_compile_definitions(${PROJECT_NAME} PRIVATE _HAS_EXCEPTIONS=1)
  target_compile_options(${PROJECT_NAME} PRIVATE /EHsc)
elseif(APPLE)
  target_compile_options(${PROJECT_NAME} PRIVATE -fvisibility=hidden)
endif()
