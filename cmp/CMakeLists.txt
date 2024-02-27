cmake_minimum_required(VERSION 3.15)

set(CMAKE_C_STANDARD 20)
set(CMAKE_CXX_STANDARD 20)

set(NAME cmplib)
file(GLOB SRC ${CMAKE_CURRENT_SOURCE_DIR}/src/*)
add_library(${NAME} STATIC ${SRC})
target_include_directories(${NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
